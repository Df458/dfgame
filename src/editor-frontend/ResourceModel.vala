public class ResourceModel : ListModel, Object
{
    Gee.ArrayList<ResourceEntry> list;
    string? extension;

    public ResourceModel(string? desired_extension)
    {
        list = new Gee.ArrayList<ResourceEntry>();
        extension = desired_extension;
    }

    public Object? get_item(uint position)
    {
        if(position >= list.size)
            return null;

        return list.get((int)position);
    }

    public Type get_item_type()
    {
        return typeof(string);
    }

    public uint get_n_items()
    {
        return list.size;
    }

    public Object? get_object(uint position)
    {
        if(position >= list.size)
            return null;

        return list.get((int)position);
    }

    public void append(File f)
    {
        list.add(new ResourceEntry(f));
    }
}
