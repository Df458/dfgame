namespace DFGame.Graphics
{
    [CCode(cname = "rawtex", destroy_function = "rawtex_cleanup", has_type_id = false)]
    [Compact]
    public struct RawTex
    {
        public uint16 width;
        public uint16 height;
        public uint8 elements;

        public uint8[] clone_data() {
            uint8[] temp = _data[0:width * height * elements - 1];
            return (owned) temp;
        }

        [CCode(cname = "data", array_length = false)]
        uint8[] _data;
    }
}
