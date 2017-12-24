applicationgamesrc  = [ 'input.c', 'mainloop.c', 'window.c' ]
applicationgamelib  = static_library('dfgame_application_game', applicationgamesrc,
                         include_directories : applicationinc,
                         dependencies : applicationdeps,
                         link_with : applicationlink,
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : applicationgamelib,
                    version : '1.0.0',
                    name : 'dfgame-application-game',
                    filebase : 'dfgame-application-game',
                    requires : ['glfw3', 'glew', 'dfgame-core', 'dfgame-gameplay', 'dfgame-graphics', 'dfgame-math'],
                    description : 'dfgame application module (game), provides helper functions to be used in a final game executable')
