applicationeditorcsrc  = [ 'input.c', 'window.c' ]
applicationeditorclib  = static_library('dfgame_application_editor_backend', applicationeditorcsrc,
                         include_directories : applicationinc,
                         dependencies : applicationdeps + [ gtk ],
                         link_with : applicationlink,
                         link_args : args,
                         install : true)


applicationeditordeps = [ gtk ]
applicationeditorsrc  = [ 'Viewport.vala' ]
applicationeditorvapi  = [ 'editorc.vapi' ]
applicationeditorinc  = [ include_directories('.') ] + applicationinc
applicationeditorlib  = shared_library('dfgame_application_editor', applicationeditorsrc + applicationeditorvapi,
                        include_directories : applicationeditorinc,
                        vala_header : 'dfgame-application-editor.h',
                        vala_vapi : 'dfgame-application-editor.vapi',
                        dependencies : applicationeditordeps,
                        link_with : [ gameplaylib, graphicslib, applicationeditorclib ],
                        vala_args : ['--target-glib=2.38'],
                        install : true,
                       install_dir : [ true, true, true ])

pkgconfig.generate(libraries : applicationeditorclib,
                    version : '1.0.0',
                    name : 'dfgame-application-editor-backend',
                    filebase : 'dfgame-application-editor-backend',
                    extra_cflags : [ '-I${prefix}/include/dfgame/application', '-I${prefix}/include/dfgame/application/editor' ],
                    requires : [ 'dfgame-gameplay', 'dfgame-graphics' ],
                    description : 'dfgame application module (editor backend code), provides classes and UI widgets for use in tools')

pkgconfig.generate(libraries : applicationeditorlib,
                    version : '1.0.0',
                    name : 'dfgame-application-editor',
                    filebase : 'dfgame-application-editor',
                    extra_cflags : [ '-I${prefix}/include/dfgame/application', '-I${prefix}/include/dfgame/application/editor' ],
                    requires : [ 'dfgame-application-editor-backend' ],
                    description : 'dfgame application module (editor), provides classes and UI widgets for use in tools')

install_data(sources : '../../../data/catalog/dfgame-editor.xml', install_dir : '/usr/share/glade/catalogs')

application_editor_c = declare_dependency(include_directories : [include_directories('.'), include_directories('..')], link_with : applicationeditorclib)
application_editor_vala = declare_dependency(include_directories : [include_directories('.'), include_directories('..')], link_with : applicationeditorlib, sources : applicationeditorvapi)
