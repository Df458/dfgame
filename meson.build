project('dfgame', 'c', 'vala',
        version : '0.0.1',
        license : 'MIT',
        default_options : ['c_std=gnu11'])

compiler = meson.get_compiler('c')

m = compiler.find_library('m', required : false)
freetype = dependency('freetype2')
glfw = dependency('glfw3')
glew = dependency('glew')
jpeg = dependency('libjpeg')
png = dependency('libpng')
tiff = dependency('libtiff-4')
tga = compiler.find_library('libtga')
openal = dependency('openal')
ogg = dependency('ogg')
vorbis = dependency('vorbisfile')

args = []

subdir('tools/glslc')

subdir('src/core')
subdir('src/math')
subdir('src/gameplay')
subdir('src/graphics')
subdir('src/application')
subdir('src/audio')
subdir('src/resource')
#subdir('src/editor')

subdir('demo/basic')

run_command('ctags', '-R', '.')

#pkgconfig = import('pkgconfig')
#pkgconfig.generate(libraries : ,
#                    version : '0.0.1',
#                    name : '',
#                    filebase : '',
#                    description : '')
