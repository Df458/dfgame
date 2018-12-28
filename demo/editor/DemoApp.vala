public class DemoApp : Gtk.Application {
    public DemoApp () {
        activate.connect (activate_response);
        shutdown.connect (shutdown_response);
    }

    private void activate_response () {
        MainWindow win = new MainWindow ();

        this.add_window (win);
        win.present ();

        win.prepare ();
    }

    private void shutdown_response () {
        Demo.cleanup ();
    }
}
