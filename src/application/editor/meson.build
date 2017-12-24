applicationeditorcsrc  = [ 'input.c', 'window.c' ]
applicationeditorclib  = static_library('dfgame_application_editor_c', applicationeditorcsrc,
                         include_directories : applicationinc,
                         dependencies : applicationdeps + [ gtk ],
                         link_with : applicationlink,
                         link_args : args,
                         install : false)

applicationeditordeps = [ gtk ]
applicationeditorsrc  = [ 'Viewport.vala']
applicationeditorinc  = [ include_directories('.') ] + applicationinc
applicationeditorlib  = shared_library('dfgame_application_editor', applicationeditorsrc,
                        include_directories : applicationeditorinc,
                        dependencies : applicationeditordeps,
                        link_with : [ gameplaylib, graphicslib, applicationeditorclib ],
                        vala_args : ['--target-glib=2.38', '--vapidir=../src/application/editor', '--pkg=editorc'],
                        install : true)

pkgconfig.generate(libraries : applicationeditorlib,
                    version : '1.0.0',
                    name : 'dfgame-application-editor',
                    filebase : 'dfgame-application-editor',
                    extra_cflags : [ '-I${prefix}/include/dfgame/application', '-I${prefix}/include/dfgame/application/editor' ],
                    requires : [ 'dfgame-gameplay', 'dfgame-graphics' ],
                    description : 'dfgame application module (editor), provides classes and UI widgets for use in tools')

install_data(sources : '../../../data/catalog/dfgame-editor.xml', install_dir : '/usr/share/glade/catalogs')

install_headers(
    [ 'window.h' ],
    subdir : 'dfgame/application/editor')
