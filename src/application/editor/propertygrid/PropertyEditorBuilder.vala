
namespace DFGame {
    public interface IPropertyBuilder : Object {
        public abstract PropertyEditor create_editor (string owner, string name, PropertyType prop, string value);
    }

    public class BasicPropertyBuilder : IPropertyBuilder, Object {
        public PropertyEditor create_editor (string owner, string name, PropertyType prop, string value) {
            switch (prop.name) {
                case "xs:float":
                    return new FloatPropertyEditor (owner, name, prop, value);

                case "xs:Boolean":
                    return new BoolPropertyEditor (owner, name, prop, value);

                case "df:color4":
                    return new ColorPropertyEditor (owner, name, prop, value);
            }

            return new GenericPropertyEditor (owner, name, prop, value);
        }
    }
}
