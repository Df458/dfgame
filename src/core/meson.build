coredeps = []
coresrc  = [ 'check.c', 'log/log.c', 'memory/alloc.c', 'container/array.c', 'container/hashmap.c', 'power.c', 'crc.c', 'stringutil.c' ]
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
                    extra_cflags : [ '-I${prefix}/include/dfgame/core' ],
                    description : 'dfgame core module, provides core functionality')

install_headers(
    [ 'check.h', 'crc.h', 'power.h', 'stringutil.h', 'types.h' ],
    subdir : 'dfgame/core')
install_headers(
    [ 'container/array.h', 'container/delegate.h', 'container/hashmap.h', 'container/array.hd', 'container/hashmap.hd' ],
    subdir : 'dfgame/core/container')
install_headers(
    [ 'log/log.h' ],
    subdir : 'dfgame/core/log')
install_headers(
    [ 'memory/alloc.h' ],
    subdir : 'dfgame/core/memory')

core = declare_dependency(include_directories : coreinc + [include_directories('.')], link_with : corelib)
