using DFGame.PropertyGrid.Editors;

namespace DFGame.PropertyGrid {
    // Interface for creating property editors
    public interface IPropertyBuilder : Object {
        public abstract PropertyEditor create_editor (string owner, Attribute attr, string value);
    }

    // Default implementation of IPropertybuilder
    public class BasicPropertyBuilder : IPropertyBuilder, Object {
        public virtual PropertyEditor create_editor (string owner, Attribute attr, string value) {
            // Handle non-primitive types
            // TODO: Complex types
            if (attr.prop_type is SimpleType) {
                SimpleType simple = (SimpleType)attr.prop_type;

                if (simple.restriction != null) {
                    // If there are enum values, use the enum editor class
                    if (simple.restriction.enum_values.length > 0) {
                        return new EnumPropertyEditor (owner, attr, value);
                    }
                }
            }

            switch (attr.prop_type.primitive_name) {
                case "xs:float":
                    return new FloatPropertyEditor (owner, attr, value);

                case "xs:Boolean":
                    return new BoolPropertyEditor (owner, attr, value);

                case "df:color4":
                    return new ColorPropertyEditor (owner, attr, value);
            }

            // Fallback to text
            return new GenericPropertyEditor (owner, attr, value);
        }
    }
}
