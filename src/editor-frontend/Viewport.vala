namespace DF {
public class Viewport : Gtk.GLArea
{
    public bool first_run = true;

    public virtual bool initialize()
    {
        return true;
    }

    public virtual bool prepare()
    {
        this.make_current();
        if(first_run && !initialize()) {
            return false;
        }

        return true;
    }

    public virtual bool render_view()
    {
        return true;
    }

    public signal void ready();
}
}
