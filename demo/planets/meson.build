dplanetsdeps = [ m ]
dplanetssrc  = [ 'main.c' ]
dplanetsinc  = [ include_directories('../../src'), include_directories('../../src/audio'), include_directories('../../src/core'), include_directories('../../src/application'), include_directories('../../src/gameplay'), include_directories('../../src/graphics'), include_directories('../../src/math'), include_directories('../../src/resource') ]
dplanetsexe  = executable('demo_planets', dplanetssrc,
                       include_directories : dplanetsinc,
                       dependencies : dplanetsdeps,
                       link_with : [ corelib, applicationlib, audiolib, graphicslib, mathlib, resourcelib ],
                       link_args : args,
                       install : false)

run_command('ctags', '-R', '.')
run_command('cp', '-rf', 'assets', '../../build/demo/planets/assets')
