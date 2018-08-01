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
                    extra_cflags : [ '-I${prefix}/include/dfgame/application', '-I${prefix}/include/dfgame/application/game' ],
                    requires : ['glfw3', 'glew', 'dfgame-core', 'dfgame-gameplay', 'dfgame-graphics', 'dfgame-math'],
                    description : 'dfgame application module (game), provides helper functions to be used in a final game executable')

install_headers(
    [ 'mainloop.h', 'mainloop.hd' ],
    subdir : 'dfgame/application/game')

application_game = declare_dependency(include_directories : [include_directories('.'), include_directories('..')], link_with : applicationgamelib)

if tidy.found()
    run_target(
        'tidy-application',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + applicationgamesrc)
endif
