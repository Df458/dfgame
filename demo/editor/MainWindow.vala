using DFGame;
using Gtk;

[GtkTemplate (ui="/org/df458/EditorDemo/MainWindow.ui")]
public class MainWindow : ApplicationWindow
{
    private Demo.Triangle triangle;

    [GtkChild]
    private Scale angle_scale;
    [GtkChild]
    private Scale size_scale;
    [GtkChild]
    private ColorButton color_selector;
    [GtkChild]
    private Switch fill_switch;
    [GtkChild]
    private DFGame.Viewport viewport;

    public MainWindow()
    {
        color_selector.notify["rgba"].connect(color_changed);
        viewport.update_interval = 60;
        viewport.update_step.connect(on_loop);
    }

    public void prepare()
    {
        viewport.make_current();

        Demo.init();
        triangle = new Demo.Triangle();
    }

    private bool on_loop(float dt)
    {
        triangle.draw();
        return true;
    }

    [GtkCallback]
    public bool on_render(Gdk.GLContext ctx)
    {
        triangle.draw();

        return true;
    }

    [GtkCallback]
    public void angle_changed()
    {
        triangle.angle = (float)angle_scale.adjustment.value;
        viewport.queue_render();
    }

    [GtkCallback]
    public void size_changed()
    {
        triangle.size = (float)size_scale.adjustment.value;
        viewport.queue_render();
    }

    public void color_changed(ParamSpec p)
    {
        triangle.color.r = (float)color_selector.rgba.red;
        triangle.color.g = (float)color_selector.rgba.green;
        triangle.color.b = (float)color_selector.rgba.blue;
        viewport.queue_render();
    }

    [GtkCallback]
    public bool toggle_filled(bool fill)
    {
        triangle.filled = fill;
        viewport.queue_render();

        return false;
    }

    [GtkCallback]
    public void on_reset()
    {
        angle_scale.adjustment.value = 0;
        size_scale.adjustment.value = 0.5;
        color_selector.rgba = Gdk.RGBA(){ red=1, green=1, blue=1, alpha=1 };
        fill_switch.active = true;
    }
}
