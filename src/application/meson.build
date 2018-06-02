applicationdeps = [ glfw, glew ]
applicationlink = [ corelib, gameplaylib, graphicslib, mathlib ]
applicationinc  = [ include_directories('..'), include_directories('../core'), include_directories('../gameplay'), include_directories('../graphics'), include_directories('../math'), include_directories('../application') ]

subdir('editor')
subdir('game')

install_headers(
    [ 'input.h', 'input_id.h', 'window_common.h' ],
    subdir : 'dfgame/application')
