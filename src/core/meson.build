coredeps = []
coresrc  = [ 'log/log.c', 'memory/alloc.c', 'container/array.c', 'container/hashmap.c', 'power.c', 'crc.c' ]
coreinc  = [ include_directories('log'), include_directories('memory'), include_directories('container') ]
corelib  = static_library('dfgame_core', coresrc,
                         include_directories : coreinc,
                         dependencies : coredeps,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
