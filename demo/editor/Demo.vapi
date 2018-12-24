[CCode (cheader_filename = "triangle.h")]
namespace Demo
{
    [CCode (cname = "init")]
    public static void init();
    [CCode (cname = "cleanup")]
    public static void cleanup();

    [CCode (cname = "triangle", free_function = "triangle_free")]
    [Compact]
    public class Triangle
    {
        [CCode (cname = "size")]
        public float size;
        [CCode (cname = "angle")]
        public float angle;
        [CCode (cname = "color")]
        public Color color;
        [CCode (cname = "filled")]
        public bool  filled;

        [CCode (cname = "triangle_init")]
        public Triangle();

        [CCode (cname = "triangle_draw")]
        public void draw();

        [CCode (cname = "triangle_load")]
        public void load(string data);

        [CCode (cname = "triangle_save")]
        public string save();
    }

    [CCode (cname = "vec4")]
    public struct Color
    {
        [CCode (cname="x")]
        float r;
        [CCode (cname="y")]
        float g;
        [CCode (cname="z")]
        float b;
        [CCode (cname="w")]
        float a;
    }
}
