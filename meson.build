project('dfgame', 'c', 'vala',
        version : '0.0.1',
        license : 'MIT',
        default_options : ['c_std=c11'])

compiler = meson.get_compiler('c')

m = compiler.find_library('m', required : false)
glfw = dependency('glfw3')
glew = dependency('glew')

args = []

subdir('tools/glslc')

subdir('src/core')
subdir('src/math')
subdir('src/gameplay')
subdir('src/graphics')
subdir('src/application')
#subdir('src/audio')
#subdir('src/editor')
#subdir('src/resource')

subdir('demo/basic')

run_command('ctags', '-R', '.')

#pkgconfig = import('pkgconfig')
#pkgconfig.generate(libraries : ,
#                    version : '0.0.1',
#                    name : '',
#                    filebase : '',
#                    description : '')
