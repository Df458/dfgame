audiodeps = [ openal ]
audiosrc  = [ 'audio_log.c', 'audio_player.c', 'audio_source.c', 'audio_system.c' ]
audioinc  = [ include_directories('../core'), include_directories('../math') ]
audiolib   = static_library('dfgame_audio', audiosrc,
                         include_directories : audioinc,
                         dependencies : audiodeps,
                         link_with: [ corelib, mathlib ],
                         link_args : args,
                         install : true)

run_command('ctags', '-R', '.')

pkgconfig.generate(libraries : audiolib,
                    version : '1.0.0',
                    name : 'dfgame-audio',
                    filebase : 'dfgame-audio',
                    requires : ['openal', 'dfgame-core', 'dfgame-math'],
                    description : 'dfgame audio module, provides audio functionality')
