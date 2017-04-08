shaders = [ glsl_gen.process('../../data/shaders/shader_basic_tex.gl'), glsl_gen.process('../../data/shaders/shader_text.gl'), glsl_gen.process('../../data/shaders/shader_basic_untex.gl') ]

graphicsdeps = [ glew ]
graphicssrc  = [ 'camera.c', 'graphics_log.c', 'shader.c', 'shader_init.c', 'mesh.c' ]
graphicsinc  = [ include_directories('../core'), include_directories('../math') ]
graphicslib  = static_library('dfgame_graphics', shaders, graphicssrc,
                         include_directories : graphicsinc,
                         dependencies : graphicsdeps,
                         link_with: [ corelib, mathlib ],
                         link_args : args,
                         install : true)
