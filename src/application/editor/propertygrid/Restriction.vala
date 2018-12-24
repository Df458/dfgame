using DFGame.Core;
using Xml;

namespace DFGame.PropertyGrid {
    // How to handle whitespace chars in a property
    public enum Whitespace {
        PRESERVE,
        REPLACE,
        COLLAPSE
    }

    // Represents an enumerated value in an XSD restriction
    public struct RestrictionEnum {
        // The name to display in UI and documentation
        string display_name;

        // The enum value
        string value;

        public RestrictionEnum (Xml.Node* node) {
            value = node->get_prop ("value");
            display_name = node->get_ns_prop (DF_DISPLAY_NAME, DF_NS) ?? value;
        }
    }

    // Represents an XSD type restriction
    public class Restriction {
        // The name of the type being based on
        public string base_name { get; private set; }

        // The set of enumerated values that are allowed
        public RestrictionEnum?[] enum_values { get; private set; }

        // The minimum value (inclusive)
        public double? min_inclusive { get; private set; default = null; }

        // The maximum value (inclusive)
        public double? max_inclusive { get; private set; default = null; }

        // The minimum value (exclusive)
        public double? min_exclusive { get; private set; default = null; }

        // The maximum value (exclusive)
        public double? max_exclusive { get; private set; default = null; }

        // The number of allowed digits
        public int? total_digits { get; private set; default = null; }

        // The number of allowed digits after the decimal point
        public int? fraction_digits { get; private set; default = null; }

        // The minimum string length
        public int? min_length { get; private set; default = null; }

        // The maximum string length
        public int? max_length { get; private set; default = null; }

        // The pattern that the string value must match
        public string? pattern { get; private set; default = null; }

        // How to handle whitespace in the string value
        public Whitespace white_space { get; private set; default = Whitespace.PRESERVE; }

        public Restriction (Xml.Node* node_dat) {
            base_name = node_dat->get_prop ("base");

            var values = new Gee.ArrayList<RestrictionEnum?> ();
            for (Xml.Node* node = node_dat->first_element_child ();
                node != null;
                node = node->next_element_sibling ()) {
                switch (node->name) {
                    case XSD_ENUMERATION:
                        values.add (RestrictionEnum (node));
                    break;
                    case XSD_MIN_INCLUSIVE:
                        double res;
                        string str;
                        if (double.try_parse (node->get_prop ("value"), out res, out str)) {
                            min_inclusive = res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_MIN_INCLUSIVE,
                                str);
                        }
                    break;
                    case XSD_MAX_INCLUSIVE:
                        double res;
                        string str;
                        if (double.try_parse (node->get_prop ("value"), out res, out str)) {
                            max_inclusive = res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_MAX_INCLUSIVE,
                                str);
                        }
                    break;
                    case XSD_MIN_EXCLUSIVE:
                        double res;
                        string str;
                        if (double.try_parse (node->get_prop ("value"), out res, out str)) {
                            min_exclusive = res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_MIN_EXCLUSIVE,
                                str);
                        }
                    break;
                    case XSD_MAX_EXCLUSIVE:
                        double res;
                        string str;
                        if (double.try_parse (node->get_prop ("value"), out res, out str)) {
                            max_exclusive = res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_MAX_EXCLUSIVE,
                                str);
                        }
                    break;
                    case XSD_TOTAL_DIGITS:
                        int64 res; // int.try_parse doesn't exist, somehow
                        string str;
                        if (int64.try_parse (node->get_prop ("value"), out res, out str)) {
                            total_digits = (int)res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_TOTAL_DIGITS,
                                str);
                        }
                    break;
                    case XSD_FRACTION_DIGITS:
                        int64 res; // int.try_parse doesn't exist, somehow
                        string str;
                        if (int64.try_parse (node->get_prop ("value"), out res, out str)) {
                            fraction_digits = (int)res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_FRACTION_DIGITS,
                                str);
                        }
                    break;
                    case XSD_LENGTH:
                        int64 res; // int.try_parse doesn't exist, somehow
                        string str;
                        if (int64.try_parse (node->get_prop ("value"), out res, out str)) {
                            min_length = (int)res;
                            max_length = (int)res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_LENGTH,
                                str);
                        }
                    break;
                    case XSD_MIN_LENGTH:
                        int64 res; // int.try_parse doesn't exist, somehow
                        string str;
                        if (int64.try_parse (node->get_prop ("value"), out res, out str)) {
                            min_length = (int)res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_MIN_LENGTH,
                                str);
                        }
                    break;
                    case XSD_MAX_LENGTH:
                        int64 res; // int.try_parse doesn't exist, somehow
                        string str;
                        if (int64.try_parse (node->get_prop ("value"), out res, out str)) {
                            max_length = (int)res;
                        } else {
                            Logger.error ("Failed to parse %s restriction: %s is not a valid number",
                                XSD_MAX_LENGTH,
                                str);
                        }
                    break;
                    case XSD_PATTERN:
                        // TODO
                    break;
                    case XSD_WHITESPACE:
                        // TODO
                    break;
                    default:
                        Logger.warn ("Unrecognized restriction content %s", node->name);
                    break;
                }
            }

            enum_values = values.to_array ();
        }
    }
}
