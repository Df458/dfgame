resourcedeps = [ vorbis, xml ]
if get_option('enable_jpeg') == true
    resourcedeps += jpeg
endif
if get_option('enable_png') == true
    resourcedeps += png
endif
if get_option('enable_tiff') == true
    resourcedeps += tiff
endif
if get_option('enable_tga') == true
    resourcedeps += tga
endif

resourcesrc  = [ 'audio_loader.c', 'data_loader.c', 'font_loader.c', 'mesh_loader.c', 'paths.c', 'sprite_loader.c', 'texture_loader.c' ]
resourceinc  = [ include_directories('../audio'), include_directories('../core'), include_directories('../graphics'), include_directories('../math') ]
resourcelib   = static_library('dfgame_resource', resourcesrc,
                         include_directories : resourceinc,
                         dependencies : resourcedeps,
                         link_with: [ audiolib, corelib, graphicslib, mathlib ],
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
