gameplaydeps = []
gameplaysrc  = [ 'control.c' ]
gameplayinc  = [ include_directories('../core'), include_directories('../math') ]
gameplaylib  = static_library('dfgame_gameplay', gameplaysrc,
                         include_directories : gameplayinc,
                         dependencies : gameplaydeps,
                         link_with : [ corelib, mathlib ],
                         link_args : args,
                         install : true)
