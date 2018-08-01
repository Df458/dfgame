audiodeps = [ openal ]
audiosrc  = [ 'audio_log.c', 'audio_player.c', 'audio_source.c', 'audio_system.c' ]
audioinc  = [ include_directories('..'), include_directories('../core'), include_directories('../math') ]
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
                    extra_cflags : [ '-I${prefix}/include/dfgame/audio' ],
                    requires : ['openal', 'dfgame-core', 'dfgame-math'],
                    description : 'dfgame audio module, provides audio functionality')

install_headers(
    [ 'audio_log.h', 'audio_player.h', 'audio_source.h', 'audio_system.h', 'audio_source.hd', 'audio_player.hd' ],
    subdir : 'dfgame/audio')

audio = declare_dependency(include_directories : include_directories('.'), link_with : audiolib)

if tidy.found()
    run_target(
        'tidy-audio',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + audiosrc)
endif
