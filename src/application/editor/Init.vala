namespace DFGame.Application {
    public static void init () {
        typeof (AdvancedExpander).ensure ();
        typeof (DFGame.PropertyGrid.PropertyGrid).ensure ();
        typeof (Vec2Entry).ensure ();
        typeof (Vec3Entry).ensure ();
        typeof (Vec4Entry).ensure ();
        typeof (Viewport).ensure ();
    }
}
