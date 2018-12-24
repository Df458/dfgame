using DFGame.PropertyGrid.Editors;

namespace DFGame.PropertyGrid {
    // Interface for creating property editors
    public interface IPropertyBuilder : Object {
        public abstract PropertyEditor create_editor (string owner, Attribute attr, string value);
    }

    // Default implementation of IPropertybuilder
    public class BasicPropertyBuilder : IPropertyBuilder, Object {
        public PropertyEditor create_editor (string owner, Attribute prop, string value) {
            string primitive_name = prop.prop_type.name;

            // Handle non-primitive types
            // TODO: Complex types
            if (prop.prop_type is SimpleType) {
                SimpleType simple = (SimpleType)prop.prop_type;

                if (simple.restriction != null) {
                    // If there are enum values, use the enum editor class
                    if (simple.restriction.enum_values.length > 0) {
                        return new EnumPropertyEditor (owner, prop, value);
                    }

                    // Restrictions are based on a type, so we collect that for later use
                    primitive_name = simple.restriction.base_name;
                }
            }

            switch (primitive_name) {
                case "xs:float":
                    return new FloatPropertyEditor (owner, prop, value);

                case "xs:Boolean":
                    return new BoolPropertyEditor (owner, prop, value);

                case "df:color4":
                    return new ColorPropertyEditor (owner, prop, value);
            }

            // Fallback to text
            return new GenericPropertyEditor (owner, prop, value);
        }
    }
}
