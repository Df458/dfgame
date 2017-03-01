basicdeps = []
basicsrc  = [ 'main.c' ]
basicinc  = [ include_directories('../../src'), include_directories('../../src/core'), include_directories('../../src/application') ]
basicexe  = executable('demo_basic', basicsrc,
                       include_directories : basicinc,
                       dependencies : basicdeps,
                       link_with : [ corelib, applicationlib, ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
