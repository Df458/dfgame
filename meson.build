project('dfgame', 'c', 'vala',
        version : '1.1.0',
        license : 'MIT',
        default_options : ['c_std=gnu11', 'werror=true'])

compiler = meson.get_compiler('c')

gnome = import('gnome')
pkgconfig = import('pkgconfig')

m = compiler.find_library('m', required : false)

tidy = find_program('clang-tidy', required: false)
lint = find_program('io.elementary.vala-lint', required: false)

cunit = dependency('cunit')
freetype = dependency('freetype2')
if get_option('enable_gif') == true
    gif = compiler.find_library('gif')
endif
gl = dependency('gl')
glfw = subproject('glfw').get_variable('glfw_dep')
glew = subproject('glew').get_variable('glew_dep')
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
gee = dependency('gee-0.8')
gtk = dependency('gtk+-3.0')

add_project_arguments('-I/usr/include/freetype2', language: 'c')

args = []
allsrc = []

subdir('tools/glslc')

subdir('src/core')
subdir('src/math')
subdir('src/gameplay')
subdir('src/graphics')
subdir('src/application')
subdir('src/audio')
subdir('src/resource')

subdir('test/core')
subdir('test/math')
subdir('test/resource')

subdir('demo/logo')
subdir('demo/comets')
subdir('demo/heightmap')
subdir('demo/fps')
subdir('demo/editor')
subdir('demo/text')

if tidy.found()
    run_target(
        'tidy',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + allsrc)
endif

run_command('ctags', '-R', '.')
