mathdeps = [ m ]
mathsrc  = [ 'aabb.c', 'interpolate.c', 'matrix.c', 'quat.c', 'transform.c', 'vector.c' ]
mathinc  = [ include_directories('../core') ]
mathlib  = static_library('dfgame_math', mathsrc,
                         include_directories : mathinc,
                         dependencies : mathdeps,
                         link_with : corelib,
                         link_args : args,
                         install : true)

pkgconfig.generate(libraries : mathlib,
                    version : '1.0.0',
                    name : 'dfgame-math',
                    filebase : 'dfgame-math',
                    extra_cflags : '-lm',
                    requires : ['dfgame-core'],
                    description : 'dfgame math module, provides various math utilities')
