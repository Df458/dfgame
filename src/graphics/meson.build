shaders = [ glsl_gen.process('../../data/shaders/shader_basic_tex.gl'), glsl_gen.process('../../data/shaders/shader_text.gl'), glsl_gen.process('../../data/shaders/shader_basic_untex.gl') ]

graphicsdeps = [ glew, freetype ]
graphicssrc  = [ 'camera.c', 'font.c', 'graphics_log.c', 'mesh.c', 'shader.c', 'shader_init.c', 'texture_atlas.c' ]
graphicsinc  = [ include_directories('../core'), include_directories('../math') ]
graphicslib  = static_library('dfgame_graphics', shaders, graphicssrc,
                         include_directories : graphicsinc,
                         dependencies : graphicsdeps,
                         link_with: [ corelib, mathlib ],
                         link_args : args,
                         install : true)
