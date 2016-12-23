mathdeps = []
mathsrc  = []
mathlib   = static_library('dfgame_math', mathsrc,
                         dependencies : mathdeps,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
