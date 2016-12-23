resourcedeps = []
resourcesrc  = []
resourcelib   = static_library('dfgame_resource', resourcesrc,
                         dependencies : resourcedeps,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
