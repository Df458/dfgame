using DFGame;

public static int main (string[] args)
{
    DemoApp app = new DemoApp();
    typeof(Viewport).ensure();
    return app.run(args);
}
