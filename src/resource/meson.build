resourcedeps = [ vorbis, xml ]
resourceargs = []
resourcepkglibs = []
resourcerequires = ['vorbisfile', 'libxml-2.0', 'dfgame-audio', 'dfgame-core', 'dfgame-graphics', 'dfgame-math']
if get_option('enable_jpeg') == true
    resourcedeps += jpeg
    resourceargs += '-Denable_jpeg'
    resourcerequires += 'libjpeg'
endif
if get_option('enable_png') == true
    resourcedeps += png
    resourceargs += '-Denable_png'
    resourcerequires += 'libpng'
endif
if get_option('enable_tiff') == true
    resourcedeps += tiff
    resourceargs += '-Denable_tiff'
    resourcerequires += 'libtiff-4'
endif
if get_option('enable_tga') == true
    resourcedeps += tga
    resourceargs += '-Denable_tga'
    resourcepkglibs += '-ltga'
endif

resourcesrc  = [ 'audio_loader.c', 'data_loader.c', 'font_loader.c', 'mesh_loader.c', 'paths.c', 'sprite_loader.c', 'texture_loader.c', 'xmlutil.c' ]
resourceinc  = [ include_directories('..'), include_directories('../audio'), include_directories('../core'), include_directories('../graphics'), include_directories('../math') ]
resourcelib  = static_library('dfgame_resource', resourcesrc,
                         include_directories : resourceinc,
                         dependencies : resourcedeps,
                         link_with: [ audiolib, corelib, graphicslib, mathlib ],
                         link_args : args,
                         c_args : resourceargs,
                         install : true)

run_command('ctags', '-R', '.')

pkgconfig.generate(resourcelib,
                    version : '1.0.0',
                    name : 'dfgame-resource',
                    filebase : 'dfgame-resource',
                    extra_cflags : [ '-I${prefix}/include/dfgame/resource' ],
                    requires : resourcerequires,
                    libraries : ['-ltga', '-ldfgame_resource' ],
                    description : 'dfgame resource module, loads assets for other modules')

install_headers(
    [ 'audio_loader.h', 'data_loader.h', 'font_loader.h', 'mesh_loader.h', 'paths.h', 'sprite_loader.h', 'texture_loader.h', 'xmlutil.h' ],
    subdir : 'dfgame/resource')

resource = declare_dependency(include_directories : include_directories('.', '..'), link_with : resourcelib)
