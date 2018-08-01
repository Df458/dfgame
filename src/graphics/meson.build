shaders = [
    glsl_gen.process('../../data/shaders/shader_debug.gl'),
    glsl_gen.process('../../data/shaders/shader_basic_tex.gl'),
    glsl_gen.process('../../data/shaders/shader_text.gl'),
    glsl_gen.process('../../data/shaders/shader_basic_untex.gl'),
]

graphicsdeps = [ gl, glew, freetype ]
graphicssrc  = [ 'camera.c', 'debug_draw.c', 'font.c', 'framebuffer.c', 'graphics_log.c', 'mesh.c', 'renderpass.c', 'shader.c', 'shader_init.c', 'sprite.c', 'spriteset.c', 'text.c', 'texture.c', 'texture_atlas.c' ]
graphicsinc  = [ include_directories('..'), include_directories('../core'), include_directories('../math') ]
graphicslib  = static_library('dfgame_graphics', shaders, graphicssrc,
                         include_directories : graphicsinc,
                         dependencies : graphicsdeps,
                         link_with: [ corelib, mathlib ],
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : graphicslib,
                    version : '1.0.0',
                    name : 'dfgame-graphics',
                    filebase : 'dfgame-graphics',
                    extra_cflags : [ '-I${prefix}/include/dfgame/graphics' ],
                    requires : ['glew', 'freetype2', 'dfgame-core', 'dfgame-math'],
                    description : 'dfgame graphics module, provides rendering functions')

install_headers(
    [ 'camera.h', 'color.h', 'debug_draw.h', 'font.h', 'framebuffer.h', 'graphics_log.h', 'mesh.h', 'renderpass.h', 'shader.h', 'shader_init.h', 'sprite.h', 'spriteset.h', 'text.h', 'texture.h', 'texture_atlas.h', 'camera.hd', 'font.hd', 'mesh.hd', 'renderpass.hd', 'shader.hd', 'sprite.hd', 'spriteset.hd', 'text.hd', 'texture_atlas.hd', 'vertex.hd' ],
    subdir : 'dfgame/graphics')

graphics = declare_dependency(include_directories : include_directories('.'), link_with : graphicslib)

if tidy.found()
    run_target(
        'tidy-graphics',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + graphicssrc)
endif
