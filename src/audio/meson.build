audiodeps = [ corelib ]
audiosrc  = []
audiolib   = static_library('dfgame_audio', audiosrc,
                         dependencies : audiodeps,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
