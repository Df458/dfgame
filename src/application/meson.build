applicationdeps = [ glfw ]
applicationsrc  = [ 'mainloop.c' ]
applicationinc  = [ include_directories('../core') ]
applicationlib  = static_library('dfgame_application', applicationsrc,
                         include_directories : applicationinc,
                         dependencies : applicationdeps,
                         link_with : corelib,
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')
