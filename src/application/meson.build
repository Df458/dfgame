applicationdeps = [ glfw, glew ]
applicationsrc  = [ 'input.c', 'mainloop.c', 'window.c' ]
applicationinc  = [ include_directories('../core'), include_directories('../gameplay'), include_directories('../graphics'), include_directories('../math') ]
applicationlib  = static_library('dfgame_application', applicationsrc,
                         include_directories : applicationinc,
                         dependencies : applicationdeps,
                         link_with : [corelib, gameplaylib, graphicslib, mathlib],
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : applicationlib,
                    version : '1.0.0',
                    name : 'dfgame-application',
                    filebase : 'dfgame-application',
                    requires : ['glfw3', 'glew', 'dfgame-core', 'dfgame-gameplay', 'dfgame-graphics', 'dfgame-math'],
                    description : 'dfgame application module, provides helper functions to be used in a final game executable')
