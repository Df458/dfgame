applicationlink = [ corelib, gameplaylib, graphicslib, mathlib ]
applicationinc  = [ include_directories('..') ]

subdir('editor')
subdir('game')

install_headers(
    [ 'input.h', 'input_id.h', 'window_common.h' ],
    subdir : 'dfgame/application')
