basicdeps = []
basicsrc  = [ 'main.c' ]
basicinc  = [ include_directories('../../src'), include_directories('../../src/core'), include_directories('../../src/application'), include_directories('../../src/graphics'), include_directories('../../src/math') ]
basicexe  = executable('demo_basic', basicsrc,
                       include_directories : basicinc,
                       dependencies : basicdeps,
                       link_with : [ corelib, applicationlib, graphicslib, mathlib ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
