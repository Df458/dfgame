mathdeps = [ m ]
mathsrc  = [ 'interpolate.c', 'matrix.c', 'quat.c', 'vector.c' ]
mathinc  = [ include_directories('../core') ]
mathlib  = static_library('dfgame_math', mathsrc,
                         include_directories : mathinc,
                         dependencies : mathdeps,
                         link_with : corelib,
                         link_args : args,
                         install : true)
