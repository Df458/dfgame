applicationdeps = []
applicationsrc  = []
applicationlib   = static_library('dfgame_application', applicationsrc,
                         dependencies : applicationdeps,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
