/* dfgame-editor-front.h generated by valac 0.30.0, the Vala compiler, do not modify */


#ifndef __SRC_DFGAME_EDITOR_FRONT_H__
#define __SRC_DFGAME_EDITOR_FRONT_H__

#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <glib-object.h>
#include <gee.h>
#include <gio/gio.h>

G_BEGIN_DECLS


#define TYPE_ASSET_TYPE (asset_type_get_type ())

#define TYPE_PROJECT_FILE_PANE (project_file_pane_get_type ())
#define PROJECT_FILE_PANE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_PROJECT_FILE_PANE, ProjectFilePane))
#define PROJECT_FILE_PANE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_PROJECT_FILE_PANE, ProjectFilePaneClass))
#define IS_PROJECT_FILE_PANE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_PROJECT_FILE_PANE))
#define IS_PROJECT_FILE_PANE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_PROJECT_FILE_PANE))
#define PROJECT_FILE_PANE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_PROJECT_FILE_PANE, ProjectFilePaneClass))

typedef struct _ProjectFilePane ProjectFilePane;
typedef struct _ProjectFilePaneClass ProjectFilePaneClass;
typedef struct _ProjectFilePanePrivate ProjectFilePanePrivate;

#define TYPE_NEW_PROJECT_DIALOG (new_project_dialog_get_type ())
#define NEW_PROJECT_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_NEW_PROJECT_DIALOG, NewProjectDialog))
#define NEW_PROJECT_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_NEW_PROJECT_DIALOG, NewProjectDialogClass))
#define IS_NEW_PROJECT_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_NEW_PROJECT_DIALOG))
#define IS_NEW_PROJECT_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_NEW_PROJECT_DIALOG))
#define NEW_PROJECT_DIALOG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_NEW_PROJECT_DIALOG, NewProjectDialogClass))

typedef struct _NewProjectDialog NewProjectDialog;
typedef struct _NewProjectDialogClass NewProjectDialogClass;
typedef struct _NewProjectDialogPrivate NewProjectDialogPrivate;

#define DF_TYPE_VIEWPORT (df_viewport_get_type ())
#define DF_VIEWPORT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DF_TYPE_VIEWPORT, DFViewport))
#define DF_VIEWPORT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DF_TYPE_VIEWPORT, DFViewportClass))
#define DF_IS_VIEWPORT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DF_TYPE_VIEWPORT))
#define DF_IS_VIEWPORT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DF_TYPE_VIEWPORT))
#define DF_VIEWPORT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DF_TYPE_VIEWPORT, DFViewportClass))

typedef struct _DFViewport DFViewport;
typedef struct _DFViewportClass DFViewportClass;
typedef struct _DFViewportPrivate DFViewportPrivate;

#define DF_TYPE_MISSING_FILES_DIALOG (df_missing_files_dialog_get_type ())
#define DF_MISSING_FILES_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DF_TYPE_MISSING_FILES_DIALOG, DFMissingFilesDialog))
#define DF_MISSING_FILES_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DF_TYPE_MISSING_FILES_DIALOG, DFMissingFilesDialogClass))
#define DF_IS_MISSING_FILES_DIALOG(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DF_TYPE_MISSING_FILES_DIALOG))
#define DF_IS_MISSING_FILES_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DF_TYPE_MISSING_FILES_DIALOG))
#define DF_MISSING_FILES_DIALOG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DF_TYPE_MISSING_FILES_DIALOG, DFMissingFilesDialogClass))

typedef struct _DFMissingFilesDialog DFMissingFilesDialog;
typedef struct _DFMissingFilesDialogClass DFMissingFilesDialogClass;
typedef struct _DFMissingFilesDialogPrivate DFMissingFilesDialogPrivate;

#define DF_TYPE_RESOURCE_SELECTOR (df_resource_selector_get_type ())
#define DF_RESOURCE_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DF_TYPE_RESOURCE_SELECTOR, DFResourceSelector))
#define DF_RESOURCE_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DF_TYPE_RESOURCE_SELECTOR, DFResourceSelectorClass))
#define DF_IS_RESOURCE_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DF_TYPE_RESOURCE_SELECTOR))
#define DF_IS_RESOURCE_SELECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DF_TYPE_RESOURCE_SELECTOR))
#define DF_RESOURCE_SELECTOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DF_TYPE_RESOURCE_SELECTOR, DFResourceSelectorClass))

typedef struct _DFResourceSelector DFResourceSelector;
typedef struct _DFResourceSelectorClass DFResourceSelectorClass;
typedef struct _DFResourceSelectorPrivate DFResourceSelectorPrivate;

#define TYPE_RESOURCE_ENTRY (resource_entry_get_type ())
#define RESOURCE_ENTRY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_RESOURCE_ENTRY, ResourceEntry))
#define RESOURCE_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_RESOURCE_ENTRY, ResourceEntryClass))
#define IS_RESOURCE_ENTRY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_RESOURCE_ENTRY))
#define IS_RESOURCE_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_RESOURCE_ENTRY))
#define RESOURCE_ENTRY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_RESOURCE_ENTRY, ResourceEntryClass))

typedef struct _ResourceEntry ResourceEntry;
typedef struct _ResourceEntryClass ResourceEntryClass;

#define TYPE_RESOURCE_MODEL (resource_model_get_type ())
#define RESOURCE_MODEL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_RESOURCE_MODEL, ResourceModel))
#define RESOURCE_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_RESOURCE_MODEL, ResourceModelClass))
#define IS_RESOURCE_MODEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_RESOURCE_MODEL))
#define IS_RESOURCE_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_RESOURCE_MODEL))
#define RESOURCE_MODEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_RESOURCE_MODEL, ResourceModelClass))

typedef struct _ResourceModel ResourceModel;
typedef struct _ResourceModelClass ResourceModelClass;
typedef struct _ResourceModelPrivate ResourceModelPrivate;

#define TYPE_ASSET_VIEWER (asset_viewer_get_type ())
#define ASSET_VIEWER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_ASSET_VIEWER, AssetViewer))
#define IS_ASSET_VIEWER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_ASSET_VIEWER))
#define ASSET_VIEWER_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TYPE_ASSET_VIEWER, AssetViewerIface))

typedef struct _AssetViewer AssetViewer;
typedef struct _AssetViewerIface AssetViewerIface;

#define TYPE_ASSET_EDITOR (asset_editor_get_type ())
#define ASSET_EDITOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_ASSET_EDITOR, AssetEditor))
#define IS_ASSET_EDITOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_ASSET_EDITOR))
#define ASSET_EDITOR_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TYPE_ASSET_EDITOR, AssetEditorIface))

typedef struct _AssetEditor AssetEditor;
typedef struct _AssetEditorIface AssetEditorIface;
typedef struct _ResourceEntryPrivate ResourceEntryPrivate;

typedef enum  {
	ASSET_TYPE_NONE = 0,
	ASSET_TYPE_ACTOR,
	ASSET_TYPE_MAP,
	ASSET_TYPE_SPRITE,
	ASSET_TYPE_TILESET
} AssetType;

struct _ProjectFilePane {
	GtkBox parent_instance;
	ProjectFilePanePrivate * priv;
	gint icon_size;
};

struct _ProjectFilePaneClass {
	GtkBoxClass parent_class;
};

struct _NewProjectDialog {
	GTypeInstance parent_instance;
	volatile int ref_count;
	NewProjectDialogPrivate * priv;
};

struct _NewProjectDialogClass {
	GTypeClass parent_class;
	void (*finalize) (NewProjectDialog *self);
};

struct _DFViewport {
	GtkGLArea parent_instance;
	DFViewportPrivate * priv;
	gboolean first_run;
};

struct _DFViewportClass {
	GtkGLAreaClass parent_class;
	gboolean (*initialize) (DFViewport* self);
	gboolean (*prepare) (DFViewport* self);
	gboolean (*render_view) (DFViewport* self);
};

struct _DFMissingFilesDialog {
	GtkDialog parent_instance;
	DFMissingFilesDialogPrivate * priv;
};

struct _DFMissingFilesDialogClass {
	GtkDialogClass parent_class;
};

struct _DFResourceSelector {
	GtkBox parent_instance;
	DFResourceSelectorPrivate * priv;
};

struct _DFResourceSelectorClass {
	GtkBoxClass parent_class;
};

struct _ResourceModel {
	GObject parent_instance;
	ResourceModelPrivate * priv;
};

struct _ResourceModelClass {
	GObjectClass parent_class;
};

struct _AssetViewerIface {
	GTypeInterface parent_iface;
	gboolean (*load) (AssetViewer* self, ResourceEntry* entry);
};

struct _AssetEditorIface {
	GTypeInterface parent_iface;
	gboolean (*save) (AssetEditor* self);
	gboolean (*create) (AssetEditor* self, ResourceEntry* entry);
};

struct _ResourceEntry {
	GObject parent_instance;
	ResourceEntryPrivate * priv;
	gchar* path;
	gchar* name;
};

struct _ResourceEntryClass {
	GObjectClass parent_class;
};


GType asset_type_get_type (void) G_GNUC_CONST;
GType project_file_pane_get_type (void) G_GNUC_CONST;
ProjectFilePane* project_file_pane_new (void);
ProjectFilePane* project_file_pane_construct (GType object_type);
gchar* project_file_pane_get_selected_path (ProjectFilePane* self);
gpointer new_project_dialog_ref (gpointer instance);
void new_project_dialog_unref (gpointer instance);
GParamSpec* param_spec_new_project_dialog (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags);
void value_set_new_project_dialog (GValue* value, gpointer v_object);
void value_take_new_project_dialog (GValue* value, gpointer v_object);
gpointer value_get_new_project_dialog (const GValue* value);
GType new_project_dialog_get_type (void) G_GNUC_CONST;
NewProjectDialog* new_project_dialog_new (GtkWindow* parent, const gchar* default_path);
NewProjectDialog* new_project_dialog_construct (GType object_type, GtkWindow* parent, const gchar* default_path);
void new_project_dialog_display (NewProjectDialog* self);
void new_project_dialog_respond (NewProjectDialog* self, gint response_id);
void new_project_dialog_destroy (NewProjectDialog* self);
GType df_viewport_get_type (void) G_GNUC_CONST;
gboolean df_viewport_initialize (DFViewport* self);
gboolean df_viewport_prepare (DFViewport* self);
gboolean df_viewport_render_view (DFViewport* self);
DFViewport* df_viewport_new (void);
DFViewport* df_viewport_construct (GType object_type);
GType df_missing_files_dialog_get_type (void) G_GNUC_CONST;
DFMissingFilesDialog* df_missing_files_dialog_new (GtkWindow* win);
DFMissingFilesDialog* df_missing_files_dialog_construct (GType object_type, GtkWindow* win);
void df_missing_files_dialog_apply (DFMissingFilesDialog* self);
GType df_resource_selector_get_type (void) G_GNUC_CONST;
DFResourceSelector* df_resource_selector_new (const gchar* display_title, const gchar* desired_extension, gboolean should_allow_multiple, gboolean show_cancel);
DFResourceSelector* df_resource_selector_construct (GType object_type, const gchar* display_title, const gchar* desired_extension, gboolean should_allow_multiple, gboolean show_cancel);
GType resource_entry_get_type (void) G_GNUC_CONST;
ResourceEntry* df_resource_selector_get_selected (DFResourceSelector* self);
GeeArrayList* df_resource_selector_get_selected_full (DFResourceSelector* self);
void df_resource_selector_add_extension (DFResourceSelector* self, const gchar* ext);
void df_resource_selector_generate_list (DFResourceSelector* self);
GFile* df_get_content_directory (void);
GFile* df_file_from_resource (const gchar* ext, const gchar* name, gboolean new_file);
gboolean df_file_is_content (GFile* file);
GType resource_model_get_type (void) G_GNUC_CONST;
ResourceModel* resource_model_new (void);
ResourceModel* resource_model_construct (GType object_type);
GObject* resource_model_get_object (ResourceModel* self, guint position);
void resource_model_append (ResourceModel* self, GFile* f);
GType asset_viewer_get_type (void) G_GNUC_CONST;
GType asset_editor_get_type (void) G_GNUC_CONST;
gboolean asset_editor_save (AssetEditor* self);
gboolean asset_editor_create (AssetEditor* self, ResourceEntry* entry);
gboolean asset_viewer_load (AssetViewer* self, ResourceEntry* entry);
ResourceEntry* resource_entry_new (GFile* file);
ResourceEntry* resource_entry_construct (GType object_type, GFile* file);
ResourceEntry* resource_entry_new_from_entry (const gchar* inpath, const gchar* inname, gboolean new_file);
ResourceEntry* resource_entry_construct_from_entry (GType object_type, const gchar* inpath, const gchar* inname, gboolean new_file);
gchar* resource_entry_construct_path (ResourceEntry* self);
GFile* resource_entry_construct_file (ResourceEntry* self);


G_END_DECLS

#endif
