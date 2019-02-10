applicationeditorcsrc  = [ 'input.c', 'window.c' ]
applicationeditorclib  = static_library('dfgame_application_editor_backend', applicationeditorcsrc,
                         include_directories : applicationinc,
                         dependencies : [ gtk ],
                         link_with : applicationlink,
                         link_args : args,
                         install : true)


applicationeditordeps = [ gee, gtk, xml ]
applicationeditorsrc  = files(
  'Init.vala',
  'propertygrid/Annotation.vala',
  'propertygrid/Attribute.vala',
  'propertygrid/editors/BoolPropertyEditor.vala',
  'propertygrid/editors/ColorPropertyEditor.vala',
  'propertygrid/editors/EnumPropertyEditor.vala',
  'propertygrid/editors/FloatPropertyEditor.vala',
  'propertygrid/editors/IntPropertyEditor.vala',
  'propertygrid/editors/PropertyEditor.vala',
  'propertygrid/editors/VectorPropertyEditor.vala',
  'propertygrid/IPropertyNode.vala',
  'propertygrid/PropertyEditorBuilder.vala',
  'propertygrid/PropertyGrid.vala',
  'propertygrid/PropertyList.vala',
  'propertygrid/PropertyType.vala',
  'propertygrid/Restriction.vala',
  'propertygrid/SimpleType.vala',
  'propertygrid/xsd_def.vala',
  'VectorEntry.vala',
  'Viewport.vala',
)
applicationeditorresources = [
  gnome.compile_resources('dfgame_application_resources', 'DFGame.Application.gresources.xml', c_name: 'dfgame_application')
]
applicationeditorvapi  = files( 'editorc.vapi' ) + core_vapi + math_vapi
applicationeditorinc  = [ include_directories('.') ] + applicationinc
applicationeditorlib  = shared_library('dfgame_application_editor', applicationeditorsrc + applicationeditorresources + applicationeditorvapi,
                        include_directories : applicationeditorinc,
                        vala_header : 'dfgame-application-editor.h',
                        vala_vapi : 'dfgame-application-editor.vapi',
                        dependencies : applicationeditordeps,
                        link_with : [ gameplaylib, graphicslib, applicationeditorclib ],
                        vala_args : ['--target-glib=2.38'],
                        install : true,
                        install_dir : [ true, true, true ])

pkgconfig.generate(applicationeditorclib,
                   version : '1.0.0',
                   name : 'dfgame-application-editor-backend',
                   filebase : 'dfgame-application-editor-backend',
                   extra_cflags : [ '-I${prefix}/include/dfgame/application', '-I${prefix}/include/dfgame/application/editor' ],
                   requires : [ 'dfgame-gameplay', 'dfgame-graphics' ],
                   description : 'dfgame application module (editor backend code), provides classes and UI widgets for use in tools')

pkgconfig.generate(applicationeditorlib,
                   version : '1.0.0',
                   name : 'dfgame-application-editor',
                   filebase : 'dfgame-application-editor',
                   extra_cflags : [ '-I${prefix}/include/dfgame/application', '-I${prefix}/include/dfgame/application/editor' ],
                   requires : [ 'dfgame-application-editor-backend' ],
                   description : 'dfgame application module (editor), provides classes and UI widgets for use in tools')

install_data(sources : '../../../data/catalog/dfgame-editor.xml', install_dir : '/usr/share/glade/catalogs')

application_editor_c = declare_dependency(include_directories : [include_directories('.'), include_directories('..')], link_with : applicationeditorclib)
application_editor_vala = declare_dependency(include_directories : [include_directories('.'), include_directories('..')], link_with : applicationeditorlib, sources : applicationeditorvapi)

if lint.found()
    run_target(
        'lint-editor',
        command: [
            lint,
        ] + applicationeditorsrc)
endif

if tidy.found()
    run_target(
        'tidy-editor',
        command: [
            tidy,
            '-checks=*-hicpp-signed-bitwise',
            '-p', meson.build_root()
        ] + applicationeditorcsrc)
endif
