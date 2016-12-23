renderdeps = []
rendersrc  = []
renderlib   = static_library('dfgame_render', rendersrc,
                         dependencies : renderdeps,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
