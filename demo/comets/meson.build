dcometsdeps = [ m ]
dcometssrc  = [ 'main.c' ]
dcometsinc  = [ include_directories('../../src'), include_directories('../../src/audio'), include_directories('../../src/core'), include_directories('../../src/application'), include_directories('../../src/application/game'), include_directories('../../src/gameplay'), include_directories('../../src/graphics'), include_directories('../../src/math'), include_directories('../../src/resource') ]
dcometsexe  = executable('demo_comets', dcometssrc,
                       include_directories : dcometsinc,
                       dependencies : dcometsdeps,
                       link_with : [ corelib, applicationgamelib, audiolib, graphicslib, mathlib, resourcelib ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
run_command('ln', '-rs', 'assets', '../../build/demo/comets')
