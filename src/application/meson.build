applicationdeps = [ glfw, glew ]
applicationsrc  = [ 'mainloop.c', 'window.c' ]
applicationinc  = [ include_directories('../core'), include_directories('../graphics') ]
applicationlib  = static_library('dfgame_application', applicationsrc,
                         include_directories : applicationinc,
                         dependencies : applicationdeps,
                         link_with : [corelib, graphicslib],
                         link_args : args,
                         install : true)
