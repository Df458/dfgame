namespace DFGame.Math
{
    [CCode (cname = "aabb_2d", destroy_function = "", has_type_id = false)]
    [Compact]
    public struct Aabb2D
    {
        public Vec2 position;
        public Vec2 dimensions;

        public float x;
        public float y;
        public float width;
        public float height;

        public Vec4 vec;

        [CCode(cname = "aabb_2d_zero")]
        public const Aabb2D zero;

        [CCode (cname="aabb_2d_printstr")]
        public const string printstr;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y, width, height);
        }
    }

    [CCode (cname = "aabb_3d", destroy_function = "", has_type_id = false)]
    [Compact]
    public struct Aabb3D
    {
        Vec3 position;
        Vec3 dimensions;

        [CCode(cname = "aabb_3d_zero")]
        public const Aabb3D zero;

        [CCode (cname="aabb_3d_printstr")]
        public const string printstr;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y, width, height);
        }
    }
}
