mathdeps = [ m ]
mathsrc  = [ 'aabb.c', 'interpolate.c', 'matrix.c', 'quat.c', 'transform.c', 'vector.c' ]
mathinc  = [ include_directories('..'), include_directories('../core') ]
mathlib  = static_library('dfgame_math', mathsrc,
                         include_directories : mathinc,
                         dependencies : mathdeps,
                         link_with : corelib,
                         link_args : args,
                         install : true)

pkgconfig.generate(mathlib,
                    version : '1.0.0',
                    name : 'dfgame-math',
                    filebase : 'dfgame-math',
                    extra_cflags : [ '-I${prefix}/include/dfgame/math' ],
                    libraries : ['-lm', '-ldfgame_math' ],
                    requires : ['dfgame-core'],
                    description : 'dfgame math module, provides various math utilities')

install_headers(
    [ 'aabb.h', 'interpolate.h', 'mathutil.h', 'matrix.h', 'quat.h', 'transform.h', 'vector.h', 'matrix.hd', 'quat.hd', 'transform.hd', 'vector.hd' ],
    subdir : 'dfgame/math')

math = declare_dependency(include_directories : include_directories('.'), link_with : mathlib)

if tidy.found()
    run_target(
        'tidy-math',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + mathsrc)
endif
