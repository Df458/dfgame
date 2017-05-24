basicdeps = [ m ]
basicsrc  = [ 'main.c' ]
basicinc  = [ include_directories('../../src'), include_directories('../../src/audio'), include_directories('../../src/core'), include_directories('../../src/application'), include_directories('../../src/gameplay'), include_directories('../../src/graphics'), include_directories('../../src/math'), include_directories('../../src/resource') ]
basicexe  = executable('demo_basic', basicsrc,
                       include_directories : basicinc,
                       dependencies : basicdeps,
                       link_with : [ corelib, applicationlib, audiolib, graphicslib, mathlib, resourcelib ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
