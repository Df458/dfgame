// WebGL context
var ctx;

// Quad VBO
var quad_buffer;

// Quad shader
var quad_program;
var quad_subtex_program;
var quad_untex_program;

var FLOAT_SIZE = 4;

var quad_vs =
    "attribute vec3 i_pos;\n"
    +"attribute vec2 i_uv;\n"
    +"uniform mat4 camera;\n"
    +"uniform mat4 transform;\n"
    +"varying highp vec2 v_uv;\n"
    +"void main(void) {\n"
    +   "gl_Position = camera * transform * vec4(i_pos, 1.0);\n"
    +   "v_uv = i_uv;\n"
    +"}";
var quad_subtex_vs =
    "attribute vec3 i_pos;\n"
    +"attribute vec2 i_uv;\n"
    +"uniform mat4 camera;\n"
    +"uniform mat4 transform;\n"
    +"uniform vec4 uv_offset;\n"
    +"varying highp vec2 v_uv;\n"
    +"void main(void) {\n"
    +   "gl_Position = camera * transform * vec4(i_pos, 1.0);\n"
    +   "v_uv  = i_uv * uv_offset.zw + uv_offset.xy;\n"
    +"}";
var quad_untex_vs =
    "attribute vec3 i_pos;\n"
    +"uniform mat4 camera;\n"
    +"uniform mat4 transform;\n"
    +"void main(void) {\n"
    +    "gl_Position = camera * transform * vec4(i_pos, 1.0);\n"
    +"}";

var quad_fs =
    "uniform lowp vec4 color;\n"
    +"uniform sampler2D texture;"
    +"varying highp vec2 v_uv;\n"
    + "void main(void) {\n"
    +    "gl_FragColor = texture2D(texture, v_uv) * color;\n"
    +"}";
var quad_untex_fs =
    "uniform lowp vec4 color;\n"
    + "void main(void) {\n"
    +    "gl_FragColor = color;\n"
    +"}";

// TODO: Make this handle attributes/uniforms
function load_program(vs_path, fs_path)
{
  var vs_element = document.getElementById(vs_path);
  var fs_element = document.getElementById(fs_path);
  if(!vs_element || !fs_element)
      return null;

  var vs_source = "";
  var fs_source = "";
  var vs_element = vs_element.firstChild;
  var fs_element = fs_element.firstChild;
  while(vs_element) {
      if (vs_element.nodeType == vs_element.TEXT_NODE) {
          vs_source += vs_element.textContent;
      }
      vs_element = vs_element.nextSibling;
  }
  while(fs_element) {
      if (fs_element.nodeType == fs_element.TEXT_NODE) {
          fs_source += fs_element.textContent;
      }
      fs_element = fs_element.nextSibling;
  }

  create_program(vs_source, fs_source);
}

function create_program(vs_data, fs_data)
{

  var vs_shader = ctx.createShader(ctx.VERTEX_SHADER);
  var fs_shader = ctx.createShader(ctx.FRAGMENT_SHADER);

  ctx.shaderSource(vs_shader, vs_data);
  ctx.compileShader(vs_shader);
  // TODO: Use the error logger
  if (!ctx.getShaderParameter(vs_shader, ctx.COMPILE_STATUS)) {
      alert("An error occurred compiling the vertex shader: " + ctx.getShaderInfoLog(vs_shader));
      return null;
  }
  ctx.shaderSource(fs_shader, fs_data);
  ctx.compileShader(fs_shader);
  // TODO: Use the error logger
  if (!ctx.getShaderParameter(fs_shader, ctx.COMPILE_STATUS)) {
      alert("An error occurred compiling the fragment shader: " + ctx.getShaderInfoLog(fs_shader));
      return null;
  }


  // TODO: Link the program
  var program = ctx.createProgram();
  ctx.attachShader(program, vs_shader);
  ctx.attachShader(program, fs_shader);
  ctx.linkProgram(program);

  // TODO: use the error logger
  if (!ctx.getProgramParameter(program, ctx.LINK_STATUS)) {
    alert("Unable to initialize the shader program.");
    return null;
  }

  return program;
}

function init_renderer()
{
    var canvas = document.getElementById("dfgame-canvas");
    try {
        ctx = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
    }
    catch(e) {}
    if (!ctx) {
        alert("Unable to initialize the WebGL context. Your browser may not support it.");
        ctx = null;
        return false;
    }

    quad_buffer = ctx.createBuffer();
    ctx.bindBuffer(ctx.ARRAY_BUFFER, quad_buffer);
    var quad_verts = [
        0.5,  0.5,  0.0, 1.0, 1.0,
        -0.5, 0.5,  0.0, 0.0, 1.0,
        0.5,  -0.5, 0.0, 1.0, 0.0,
        -0.5, -0.5, 0.0, 0.0, 0.0 
    ];
    ctx.bufferData(ctx.ARRAY_BUFFER, new Float32Array(quad_verts), ctx.STATIC_DRAW);
    quad_untex_program = create_program(quad_untex_vs, quad_untex_fs);
    quad_subtex_program = create_program(quad_subtex_vs, quad_fs);
    quad_program = create_program(quad_vs, quad_fs);

    ctx.clearColor(0.0, 0.0, 0.0, 1.0);
    ctx.enable(ctx.DEPTH_TEST);
    ctx.enable(ctx.BLEND);
    ctx.blendFunc(ctx.SRC_ALPHA, ctx.ONE_MINUS_SRC_ALPHA);
    ctx.depthFunc(ctx.LEQUAL);
    ctx.clear(ctx.DEPTH_BUFFER_BIT | ctx.COLOR_BUFFER_BIT);

    return true;
}

function cleanup_renderer()
{
}

function render_quad(camera, transform)
{
    var prg = quad_untex_program;
    var world = new Matrix(transform);

    var color = [1, 1, 1, 1];
    if(arguments.length >= 6) {
        if(!arguments[3].ready)
            return;
        color = arguments[2];
        prg = quad_subtex_program

        ctx.useProgram(prg);

        var uvu = ctx.getUniformLocation(prg, "uv_offset");
        ctx.uniform4fv(uvu, arguments[4]);
        var txu = ctx.getUniformLocation(prg, "texture");
        ctx.activeTexture(ctx.TEXTURE0);
        ctx.bindTexture(ctx.TEXTURE_2D, arguments[3].tex);
        ctx.uniform1i(txu, 0);

        ctx.bindBuffer(ctx.ARRAY_BUFFER, quad_buffer);
        var vta = ctx.getAttribLocation(prg, "i_uv");
        ctx.enableVertexAttribArray(vta);
        ctx.vertexAttribPointer(vta, 2, ctx.FLOAT, false, FLOAT_SIZE * 5, FLOAT_SIZE * 3);

        if(arguments[5]) {
            var sc = new Matrix();
            sc.scale(arguments[3].width * arguments[4][2], arguments[3].height * arguments[4][3], false);
            world.mul(sc);
        }
    } else if(arguments.length >= 5) {
        if(!arguments[3].ready)
            return;
        color = arguments[2];
        prg = quad_program

        ctx.useProgram(prg);

        var txu = ctx.getUniformLocation(prg, "texture");
        ctx.activeTexture(ctx.TEXTURE0);
        ctx.bindTexture(ctx.TEXTURE_2D, arguments[3].tex);
        ctx.uniform1i(txu, 0);

        ctx.bindBuffer(ctx.ARRAY_BUFFER, quad_buffer);
        var vta = ctx.getAttribLocation(prg, "i_uv");
        ctx.enableVertexAttribArray(vta);
        ctx.vertexAttribPointer(vta, 2, ctx.FLOAT, false, FLOAT_SIZE * 5, FLOAT_SIZE * 3);
        if(arguments[4]) {
            var sc = new Matrix();
            sc.scale(arguments[3].width, arguments[3].height, false);
            world.mul(sc);
        }
    } else if(arguments.length >= 3) {
        color = arguments[2];
    }
    ctx.useProgram(prg);


    ctx.bindBuffer(ctx.ARRAY_BUFFER, quad_buffer);
    var vta = ctx.getAttribLocation(prg, "i_pos");
    ctx.enableVertexAttribArray(vta);
    ctx.vertexAttribPointer(vta, 3, ctx.FLOAT, false, FLOAT_SIZE * 5, 0);

    var tsu = ctx.getUniformLocation(prg, "transform");
    ctx.uniformMatrix4fv(tsu, false, world.data);

    var cmu = ctx.getUniformLocation(prg, "camera");
    ctx.uniformMatrix4fv(cmu, false, camera.data);
    var cvu = ctx.getUniformLocation(prg, "color");
    ctx.uniform4fv(cvu, color);

    ctx.drawArrays(ctx.TRIANGLE_STRIP, 0, 4);
}

function get_context()
{
    return ctx;
}
