glslcdeps = [ ]
glslcsrc  = [ 'main.c' ]
glslcinc  = [ ]
glslc  = executable('glslc', glslcsrc,
                   include_directories : glslcinc,
                   dependencies : glslcdeps,
                   install : true,
                   native : true)
glsl_gen = generator(glslc,
                    output: '@BASENAME@.h',
                    arguments: ['-o', '@OUTPUT@', '-d', '@OUTPUT@.d', '@INPUT@'],
                    depfile: '@BASENAME@.d')

run_command('ctags', '-R', '.')

if tidy.found()
    run_target(
        'tidy-glslc',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + glslcsrc)
endif
