using DFGame.PropertyGrid;
using Gtk;

[GtkTemplate (ui="/org/df458/EditorDemo/MainWindow.ui")]
public class MainWindow : ApplicationWindow {
    private Demo.Scene scene;

    [GtkChild]
    private PropertyGrid properties;
    [GtkChild]
    private DFGame.Viewport viewport;

    public void prepare () {
        viewport.make_current ();

        Demo.init ();
        scene = new Demo.Scene ();

        properties.builder = new BasicPropertyBuilder ();
        properties.load_schema_from_resource ("/org/df458/EditorDemo/schemas/triangle.xsd");

        properties.load_data (scene.save ());
        properties.show_all ();
    }

    [GtkCallback]
    private bool on_loop (float dt) {
        viewport.make_current ();
        scene.draw ();

        return true;
    }

    [GtkCallback]
    public void on_reset () {
        scene.reset ();
        properties.update_data (scene.save ());
    }

    [GtkCallback]
    private void triangle_property_changed (Xml.Node* data) {
        scene.load (data);
    }
}
