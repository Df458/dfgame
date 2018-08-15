dfpsdeps = [ m ]
dfpssrc  = [ 'main.c' ]
dfpsinc  = [ include_directories('../../src'), include_directories('../../src/audio'), include_directories('../../src/core'), include_directories('../../src/application'), include_directories('../../src/application/game'), include_directories('../../src/gameplay'), include_directories('../../src/graphics'), include_directories('../../src/math'), include_directories('../../src/resource') ]
dfpsexe  = executable('demo_fps', dfpssrc,
                       include_directories : dfpsinc,
                       dependencies : dfpsdeps,
                       link_with : [ corelib, applicationgamelib, audiolib, graphicslib, mathlib, resourcelib ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
run_command('ln', '-rs', 'assets', '../../build/demo/fps')
