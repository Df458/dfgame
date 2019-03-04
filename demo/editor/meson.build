deditor_cinc = [include_directories('../../src'), include_directories('../../src/math'), include_directories('../../src/core'), include_directories('../../src/graphics'), include_directories('../../src/gameplay'), include_directories('../../src/application'), include_directories('../../src/application/editor')]
deditor_cdeps = [ glew, xml ]
deditor_ctarget = static_library('demo_editor', [ 'triangle.c', 'scene.c' ],
                        dependencies : deditor_cdeps,
                        link_with : [ resourcelib ],
                        include_directories : deditor_cinc,
                        install : false)

deditorsrc = [ 'main.vala', 'DemoApp.vala', 'MainWindow.vala', 'Demo.vapi', 'Scene.vapi' ]
deditordeps = [ gee, gtk, application_editor_vala, xml ]
deditorinc = deditor_cinc
deditorresources = gnome.compile_resources('resources', 'assets/demo.gresource.xml', source_dir : 'assets')
deditortarget = executable('demo_editor', deditorsrc + deditorresources,
                        dependencies : deditordeps,
                        include_directories : deditorinc,
                        link_with : [ applicationeditorlib, gameplaylib, graphicslib, deditor_ctarget ],
                        vala_args : ['--target-glib=2.38'],
                        install : false)

run_command('ctags', '-R', '.')

if lint.found()
    run_target(
        'lint-demo-editor',
        command: [
            lint,
        ] + deditorsrc)
endif
