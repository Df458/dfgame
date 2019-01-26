cdata = configuration_data()
cdata.set('TOP_SRCDIR', join_paths(meson.source_root(), 'src'))
cdata.set('TOP_BUILDDIR', meson.build_root())
cdata.set('OUTPUT_DIR', join_paths(meson.build_root(),'doc'))
cdata.set('README_PATH', join_paths(meson.source_root(),'README.md'))
cdata.set('VERSION', meson.project_version())
cdata.set('PROJECT_NAME', meson.project_name())

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: cdata,
                          install: false)

doxygen = find_program('doxygen')

doc_target = run_target('doc', command: [doxygen, doxyfile])

valadoc = find_program('valadoc')
build_dir = join_paths(meson.build_root(), 'doc', 'valadoc')
run_target('valadoc', command : [
  valadoc,
  '--package-name', meson.project_name(),
  '--package-version', meson.project_version(),
  '-b', join_paths(meson.source_root(),'src'),
  '-o', build_dir,
  '--pkg', gee.name(),
  '--pkg', xml.name(),
  '--pkg', gtk.name(),
  '--verbose',
  '--force'
  ] + applicationeditorsrc + applicationeditorvapi,
  depends: applicationeditorlib)
