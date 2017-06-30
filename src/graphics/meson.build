shaders = [ glsl_gen.process('../../data/shaders/shader_basic_tex.gl'), glsl_gen.process('../../data/shaders/shader_text.gl'), glsl_gen.process('../../data/shaders/shader_basic_untex.gl'), glsl_gen.process('../../data/shaders/shader_test_post.gl'), glsl_gen.process('../../data/shaders/invert.gl'), glsl_gen.process('../../data/shaders/wave.gl'), glsl_gen.process('../../data/shaders/blur.gl') ]

graphicsdeps = [ glew, freetype ]
graphicssrc  = [ 'camera.c', 'font.c', 'framebuffer.c', 'graphics_log.c', 'mesh.c', 'renderpass.c', 'shader.c', 'shader_init.c', 'sprite.c', 'spriteset.c', 'text.c', 'texture.c', 'texture_atlas.c' ]
graphicsinc  = [ include_directories('../core'), include_directories('../math') ]
graphicslib  = static_library('dfgame_graphics', shaders, graphicssrc,
                         include_directories : graphicsinc,
                         dependencies : graphicsdeps,
                         link_with: [ corelib, mathlib ],
                         link_args : args,
                         install : true)
