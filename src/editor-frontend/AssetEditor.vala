public interface AssetEditor : AssetViewer, Gtk.Widget
{
    public abstract bool save();
    public abstract bool create(ResourceEntry entry);
}
