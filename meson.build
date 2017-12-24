project('dfgame', 'c', 'vala',
        version : '1.1.0',
        license : 'MIT',
        default_options : ['c_std=gnu11'])

compiler = meson.get_compiler('c')

gnome = import('gnome')
pkgconfig = import('pkgconfig')

m = compiler.find_library('m', required : false)
freetype = dependency('freetype2')
glfw = dependency('glfw3')
glew = dependency('glew')
if get_option('enable_jpeg') == true
    jpeg = dependency('libjpeg')
endif
openal = dependency('openal')
ogg = dependency('ogg')
if get_option('enable_png') == true
    png = dependency('libpng')
endif
if get_option('enable_tiff') == true
    tiff = dependency('libtiff-4')
endif
if get_option('enable_tga') == true
    tga = compiler.find_library('tga')
endif
vorbis = dependency('vorbisfile')
xml = dependency('libxml-2.0')
gtk = dependency('gtk+-3.0')

add_project_arguments('-I/usr/include/freetype2', language: 'c')

args = []

subdir('tools/glslc')

subdir('src/core')
subdir('src/math')
subdir('src/gameplay')
subdir('src/graphics')
subdir('src/application')
subdir('src/audio')
subdir('src/resource')

subdir('demo/logo')
subdir('demo/comets')
subdir('demo/heightmap')
subdir('demo/fps')
subdir('demo/editor')

run_command('ctags', '-R', '.')
