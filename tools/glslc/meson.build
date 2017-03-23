glslcdeps = [ ]
glslcsrc  = [ 'main.c' ]
glslcinc  = [ ]
glslc  = executable('glslc', glslcsrc,
                   include_directories : glslcinc,
                   dependencies : glslcdeps,
                   install : true)
glsl_gen = generator(glslc,
                    output: '@BASENAME@.h',
                    arguments: ['-o', '@OUTPUT@', '@INPUT@'])

run_command('ctags', '-R', '.')
