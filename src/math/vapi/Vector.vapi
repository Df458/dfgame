namespace DFGame.Math
{
    [CCode (cname = "vec2", cheader_filename = "math/vector.h", has_copy_function = false, has_destroy_function = false, default_value = "vec2_zero", has_type_id = false)]
    [Compact]
    public struct Vec2
    {
        public float x;
        public float y;
        public float data[2];

        [CCode (cname="vec2_printstr")]
        public const string printstr;

        [CCode (cname="vec2_forward")]
        public const Vec2 forward;
        [CCode (cname="vec2_up")]
        public const Vec2 up;
        [CCode (cname="vec2_zero")]
        public const Vec2 zero;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y);
        }
    }

    [CCode (cname = "vec3", cheader_filename = "math/vector.h", has_copy_function = false, has_destroy_function = false, has_type_id = false)]
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

        [CCode (cname="vec3_forward")]
        public const Vec3 forward;
        [CCode (cname="vec3_right")]
        public const Vec3 right;
        [CCode (cname="vec3_up")]
        public const Vec3 up;
        [CCode (cname="vec3_zero")]
        public const Vec3 zero;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y, z);
        }
    }

    [CCode (cname = "vec4", cheader_filename = "math/vector.h", has_copy_function = false, has_destroy_function = false, has_type_id = false)]
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

        [CCode (cname="vec4_forward")]
        public const Vec4 forward;
        [CCode (cname="vec4_right")]
        public const Vec4 right;
        [CCode (cname="vec4_up")]
        public const Vec4 up;
        [CCode (cname="vec4_zero")]
        public const Vec4 zero;

        [CCode (cname="vec4_printstr")]
        public const string printstr;

        [CCode (cname="vala_to_string")]
        public string to_string() {
            return printstr.printf(x, y, z, w);
        }
    }
}
