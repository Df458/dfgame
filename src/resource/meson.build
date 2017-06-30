resourcedeps = [ jpeg, png, tiff, tga, vorbis, xml ]
resourcesrc  = [ 'audio_loader.c', 'data_loader.c', 'font_loader.c', 'mesh_loader.c', 'paths.c', 'sprite_loader.c', 'texture_loader.c' ]
resourceinc  = [ include_directories('../audio'), include_directories('../core'), include_directories('../graphics'), include_directories('../math') ]
resourcelib   = static_library('dfgame_resource', resourcesrc,
                         include_directories : resourceinc,
                         dependencies : resourcedeps,
                         link_with: [ audiolib, corelib, graphicslib, mathlib ],
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
