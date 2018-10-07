namespace DFGame.Math
{
    [CCode (cname = "vec2", destroy_function = "", has_type_id = false)]
    [Compact]
    public struct Vec2
    {
        public float x;
        public float y;
        public float data[2];

        [CCode (cname="vec2_printstr")]
        public const string printstr;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y);
        }
    }

    [CCode (cname = "vec3", destroy_function = "", has_type_id = false)]
    [Compact]
    public struct Vec3
    {
        public float x;
        public float y;
        public float z;
        public float data[3];

        public Vec2 xy;

        [CCode (cname="vec3_printstr")]
        public const string printstr;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y, z);
        }
    }

    [CCode (cname = "vec4", destroy_function = "", has_type_id = false)]
    [Compact]
    public struct Vec4
    {
        public float x;
        public float y;
        public float z;
        public float w;
        public float data[4];

        public Vec3 xyz;
        public Vec2 xy;
        public Vec2 zw;

        [CCode (cname="vec4_printstr")]
        public const string printstr;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y, z, w);
        }
    }
}
