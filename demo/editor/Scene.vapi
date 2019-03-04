namespace Demo {
    [CCode (cname = "scene", cprefix="scene_", free_function = "scene_free", cheader_filename = "scene.h")]
    [Compact]
    public class Scene {

        [CCode (cname = "scene_init")]
        public Scene ();

        public void draw ();
        public void load (Xml.Node* data);
        public void reset ();
        public string save ();
    }
}
