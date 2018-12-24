using DFGame.PropertyGrid;
using Gtk;

[GtkTemplate (ui="/org/df458/EditorDemo/MainWindow.ui")]
public class MainWindow : ApplicationWindow
{
    private Demo.Triangle triangle;

    [GtkChild]
    private PropertyGrid properties;
    [GtkChild]
    private DFGame.Viewport viewport;

    public void prepare()
    {
        viewport.is_active = true;
        viewport.update_interval = 60;
        viewport.update_step.connect(on_loop);
        viewport.make_current();

        Demo.init();
        triangle = new Demo.Triangle();

        properties.builder = new BasicPropertyBuilder();
        properties.load_schema_from_resource("/org/df458/EditorDemo/schemas/triangle.xsd");

        properties.load_data(triangle.save());
        properties.show_all();
    }

    private bool on_loop(float dt)
    {
        viewport.make_current();

        triangle.draw();
        return true;
    }

    [GtkCallback]
    public void on_reset()
    {
        triangle.angle = 0;
        triangle.size = 0.5f;
        triangle.filled = true;
        triangle.color.r = 1.0f;
        triangle.color.g = 1.0f;
        triangle.color.b = 1.0f;

        properties.update_data(triangle.save());
    }

    [GtkCallback]
    private void triangle_property_changed(string data) {
        triangle.load(data);
    }
}
