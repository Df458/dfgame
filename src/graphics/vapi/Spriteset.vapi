[CCode(cheader_filename="graphics/spriteset.h")]
namespace DFGame.Graphics {
    [CCode(cname = "animation", destroy_function = "", has_type_id = false)]
    [Compact]
    public struct Animation
    {
        uint8 orient_count;

        [CCode (cname="origin.x")]
        int16 origin_x;
        [CCode (cname="origin.y")]
        int16 origin_y;
        Math.Aabb2D texture_box;

        uint16 frame_count;
        uint16 default_frame_time;
        [CCode (array_length="frame_count")]
        uint16[] frame_times;
        uint16 total_time;

        int16 texture_id;

        bool autoplay;
        bool autoloop;
        bool default_on_finish;

        string filepath;
        string name;

        [CCode (cname="animation_get_frame")]
        public bool get_frame(uint32 time, out uint16 frame, out uint32 adjusted_time);
        [CCode (cname="animation_get_time")]
        public uint32 get_time(uint16 frame);
        [CCode (cname="animation_calculate_total_time")]
        public void calculate_total_time();

        [CCode(cname = "ANIMATION_DEFAULT")]
        public const string default_name;
    }

    [CCode(cname = "spriteset", destroy_function = "_spriteset_free", has_type_id = false)]
    [Compact]
    public class Spriteset
    {
        [CCode(cname = "spriteset_new")]
        public Spriteset(string path);

        public uint16 animation_count { [CCode(cname = "spriteset_get_animation_count")] get; }
        public string filepath { [CCode(cname = "spriteset_get_path")] get; }

        [CCode(cname = "spriteset_add_animation_empty")]
        public unowned Animation* add_animation();
        [CCode(cname = "spriteset_add_animation_raw")]
        public unowned Animation* add_animation_raw(Animation anim, RawTex texture);
        [CCode(cname = "spriteset_remove_animation")]
        public void remove_animation(Animation* anim);

        [CCode(cname = "spriteset_set_animation_texture_raw")]
        public void set_animation_texture_raw(Animation* anim, RawTex texture);

        [CCode(cname = "spriteset_set_animation_name")]
        public bool set_animation_name(Animation* anim, string name);

        [CCode(cname = "spriteset_get_animation_by_name")]
        public unowned Animation* get_animation_by_name(string name);
        [CCode(cname = "spriteset_get_animation_by_id")]
        public unowned Animation* get_animation_by_id(int16 id);
    }
}
