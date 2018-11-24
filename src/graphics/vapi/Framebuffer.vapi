[CCode (cheader_filename = "graphics/renderpass.h")]
namespace DFGame.Graphics {
    public class Framebuffer {
        [CCode (cname = "renderpass_null_response_func", has_target = false)]
        public delegate void Callback(void* pass, void* user);
        [CCode (cname = "renderpass_null_response", destroy_function = "", has_type_id = false)]
        public struct CallbackData {
            [CCode (cname = "func", has_target = false)]
            Callback cb;
            [CCode (cname = "user")]
            void* user;
        }
    }
}
