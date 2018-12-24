namespace DFGame.Application {
    public static void init () {
        typeof (DFGame.PropertyGrid.PropertyGrid).ensure ();
        typeof (Viewport).ensure ();
    }
}
