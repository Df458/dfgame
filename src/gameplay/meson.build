gameplaydeps = []
gameplaysrc  = [ 'control.c' ]
gameplayinc  = [ include_directories('../core'), include_directories('../math') ]
gameplaylib  = static_library('dfgame_gameplay', gameplaysrc,
                         include_directories : gameplayinc,
                         dependencies : gameplaydeps,
                         link_with : [ corelib, mathlib ],
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : gameplaylib,
                    version : '1.0.0',
                    name : 'dfgame-gameplay',
                    filebase : 'dfgame-gameplay',
                    requires : ['dfgame-core', 'dfgame-math'],
                    extra_cflags : [ '-I${prefix}/include/dfgame/gameplay' ],
                    description : 'dfgame gameplay module, acts as a proxy between platform-specific modules')

install_headers(
    [ 'control.h' ],
    subdir : 'dfgame/gameplay')

gameplay = declare_dependency(include_directories : include_directories('.'), link_with : gameplaylib)
