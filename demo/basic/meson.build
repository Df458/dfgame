basicdeps = []
basicsrc  = [ 'main.c' ]
basicinc  = [ include_directories('../../src') ]
basicexe  = executable('demo_basic', basicsrc,
                       include_directories : basicinc,
                       dependencies : basicdeps,
                       link_with : corelib,
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
