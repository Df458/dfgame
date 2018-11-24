using DFGame.Application;

public static int main (string[] args)
{
    init();

    DemoApp app = new DemoApp();
    return app.run(args);
}
