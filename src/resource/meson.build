resourcedeps = [ vorbis, xml ]
resourceargs = []
resourcerequires = ['vorbisfile', 'libxml-2.0', 'dfgame-audio', 'dfgame-core', 'dfgame-graphics', 'dfgame-math']
if get_option('enable_jpeg') == true
    resourcedeps += jpeg
    resourceargs += '-Denable_jpeg'
    resourcerequires += 'jpeg'
endif
if get_option('enable_png') == true
    resourcedeps += png
    resourceargs += '-Denable_png'
    resourcerequires += 'png'
endif
if get_option('enable_tiff') == true
    resourcedeps += tiff
    resourceargs += '-Denable_tiff'
    resourcerequires += 'tiff'
endif
if get_option('enable_tga') == true
    resourcedeps += tga
    resourceargs += '-Denable_tga'
    resourcerequires += 'tga'
endif

resourcesrc  = [ 'audio_loader.c', 'data_loader.c', 'font_loader.c', 'mesh_loader.c', 'paths.c', 'sprite_loader.c', 'texture_loader.c' ]
resourceinc  = [ include_directories('../audio'), include_directories('../core'), include_directories('../graphics'), include_directories('../math') ]
resourcelib  = static_library('dfgame_resource', resourcesrc,
                         include_directories : resourceinc,
                         dependencies : resourcedeps,
                         link_with: [ audiolib, corelib, graphicslib, mathlib ],
                         link_args : args,
                         c_args : resourceargs,
                         install : true)

run_command('ctags', '-R', '.')

pkgconfig.generate(libraries : resourcelib,
                    version : '1.0.0',
                    name : 'dfgame-resource',
                    filebase : 'dfgame-resource',
                    requires : resourcerequires,
                    description : 'dfgame resource module, loads assets for other modules')
