dtextdeps = [ m ]
dtextsrc  = [ 'main.c' ]
dtextinc  = [ include_directories('../../src') ]
dtextexe  = executable('demo_text', dtextsrc,
                       include_directories : dtextinc,
                       dependencies : dtextdeps,
                       link_with : [ corelib, applicationgamelib, graphicslib, mathlib, resourcelib ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
run_command('cp', '-rf', 'assets', '../../build/demo/text')
