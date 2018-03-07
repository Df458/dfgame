deditor_cinc = [include_directories('../../src/math'), include_directories('../../src/core'), include_directories('../../src/graphics'), include_directories('../../src/gameplay'), include_directories('../../src/application'), include_directories('../../src/application/editor')]
deditor_cdeps = [ glew ]
deditor_ctarget = static_library('demo_editor', 'triangle.c',
                        dependencies : deditor_cdeps,
                        include_directories : deditor_cinc,
                        install : false)

deditorsrc = [ 'main.vala', 'DemoApp.vala', 'MainWindow.vala', 'Demo.vapi' ]
deditordeps = [ gtk ]
deditorinc = deditor_cinc
deditorresources = gnome.compile_resources('resources', 'assets/demo.gresource.xml', source_dir : 'assets')
deditortarget = executable('demo_editor', deditorsrc + deditorresources,
                        dependencies : deditordeps,
                        include_directories : deditorinc,
                        link_with : [ applicationeditorlib, gameplaylib, graphicslib, deditor_ctarget ],
                        vala_args : ['--target-glib=2.38'],
                        install : false)

run_command('ctags', '-R', '.')
