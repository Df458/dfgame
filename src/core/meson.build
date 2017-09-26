coredeps = []
coresrc  = [ 'check.c', 'log/log.c', 'memory/alloc.c', 'container/array.c', 'container/hashmap.c', 'power.c', 'crc.c' ]
coreinc  = [ include_directories('log'), include_directories('memory'), include_directories('container') ]
corelib  = static_library('dfgame_core', coresrc,
                         include_directories : coreinc,
                         dependencies : coredeps,
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : corelib,
                    version : '1.0.0',
                    name : 'dfgame-core',
                    filebase : 'dfgame-core',
                    description : 'dfgame core module, provides core functionality')
