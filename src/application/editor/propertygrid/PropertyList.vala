using DFGame.Application;
using DFGame.Core;
using DFGame.PropertyGrid.Editors;
using Gtk;
using Xml;

namespace DFGame.PropertyGrid {
    /**
     * Represents a list of property editors in a {@link PropertyGrid}'s hierarchy
     */
    internal class PropertyList : Bin, IPropertyNode {
        /**
         * The builder used to construct this list's {@link IPropertyEditor} children
         */
        public IPropertyBuilder builder {
            get { return _builder; }
            set {
                if (_builder != value) {
                    _builder = value;

                    foreach (PropertyList list in lists) {
                        list.builder = _builder;
                    }
                }
            }
        }
        private IPropertyBuilder _builder;

        /**
         * The number of child elements in this list
         */
        public uint child_count { get { return properties_list.get_children ().length (); } }

        /**
         * The element that this widget represents
         */
        public Element element { get; private set; }

        /**
         * Expands/collapses this widget's {@link AdvancedExpander}
         */
        public bool expanded {
            get { return expander.expanded; }
            set { expander.expanded = value; }
        }

        /**
         * Whether or not this list should deliver change events
         */
        public bool frozen {
            get { return _frozen; }
            set {
                if (_frozen != value) {
                    _frozen = value;

                    foreach (PropertyList list in lists) {
                        list.frozen = _frozen;
                    }
                }
            }
        }
        private bool _frozen;

        /**
         * Is this a collection of nodes, or a regular element?
         */
        public bool is_collection { get { return element is MultiElement; } }

        /**
         * Hint for a parent list's sorting
         */
        public uint sort_index { get; protected set; default = 0; }

        /**
         * Called when the value of an editor changes
         *
         * @param data The xml data
         */
        public signal void value_changed (Xml.Node* data);

        construct {
            expander = new AdvancedExpander (null);

            expander_content = new Box (Orientation.VERTICAL, 6);
            properties_list = new ListBox ();
            properties_list.get_style_context ().add_class (STYLE_CLASS_BACKGROUND);
            properties_list.selection_mode = SelectionMode.NONE;
            properties_list.set_sort_func (sort_properties);
            properties_list.margin_start = 12;

            expander_content.pack_end (properties_list);
            expander.body_child = expander_content;
            this.add (expander);
        }

        public PropertyList (Element? elem, Xml.Node* node, IPropertyBuilder b, PropertyList? p = null) {
            builder = b;
            parent_list = p;
            element = elem;

            if (p != null) {
                MultiElement multi = p.element as MultiElement;
                if (multi != null) {
                    var choices = multi.get_choice_elements ();
                    if (choices.size > 1) {
                        type_header = new TypeSelectionHeader (multi, elem);
                        type_header.type_changed.connect (on_type_changed);

                        expander_content.pack_start (type_header, false, false);
                    }
                    if (multi.max_occurs == 1) {
                        remove (expander);
                        expander.body_child = null;
                        add (expander_content);
                    }
                }
            }

            if (elem != null) {
                build_from_element (elem, node);
            }
        }

        /**
         * Adds a new property editor to the list
         *
         * @param attr The {@link Attribute} to get the property from
         * @param value The value of the property. If the value is null, the default value will be used.
         */
        public void add_property (Attribute attr, string? value) {
            PropertyEditor editor = builder.create_editor (
                element.name,
                attr,
                value ?? attr.default_value ?? "");

            editor.value_changed.connect (on_update_value);
            editor.set_groups (label_group, control_group);
            add_child (editor);
            widgets[attr.name] = editor;
        }

        /**
         * Updates existing properties with new values in the provided XML
         *
         * @param node The XML node to get data from
         */
        public void update_data (Xml.Node* node) {
            foreach (PropertyList list in lists) {
                if (list.is_collection) {
                    // TODO Reset
                }
            }

            for (var attr = node->properties; attr != null; attr = attr->next) {
                PropertyEditor p = widgets.get (attr->name);
                if (p != null) {
                    p.current_value = node->get_prop (attr->name);
                } else {
                    Logger.warn ("No attribute %s in %s", attr->name, element.name);
                }
            }
            for (var elem = node->children; elem != null; elem = elem->next) {
                PropertyList l = lists.first_match (e => e.element.name == elem->name);
                if (l != null) {
                    l.update_data (elem);
                } else {
                    bool found = false;
                    foreach (PropertyList list in lists) {
                        if (list.is_collection) {
                            Element match = ((MultiElement)list.element).get_choice_element (elem->name);
                            if (match != null) {
                                list.add_item (match, elem);
                                found = true;
                                break;
                            }
                        }
                    }

                    if (!found) {
                        Logger.warn ("No element %s in %s", elem->name, element.name);
                    }
                }
            }
        }

        /**
         * Get the data from this element and its children as XML
         */
        public Xml.Node* get_xml () {
            Xml.Node* n = new Xml.Node (null, element.name);
            foreach (PropertyEditor editor in widgets.values) {
                n->set_prop (editor.property_name, editor.current_value);
            }
            foreach (PropertyList list in lists) {
                Xml.Node* child_node = list.get_xml ();
                if (element is MultiElement) {
                    var row = list.parent as ListBoxRow;
                    child_node->set_prop (DF_INDEX, row.get_index ().to_string ());
                }
                n->add_child (child_node);
            }

            return n;
        }

        /**
         * Add a child widget to the list
         */
        private void add_child (Widget w) {
            var row = new ListBoxRow ();
            row.activatable = false;
            row.selectable = false;
            row.add (w);
            properties_list.add (row);
        }

        private void on_update_value (PropertyEditor editor, string value) {
            if (!frozen) {
                Xml.Node* n = new Xml.Node (null, element.name);
                n->set_prop (editor.property_name, value);

                value_changed (n);
            }
        }

        private void on_update_value_xml (PropertyList list, Xml.Node* value) {
            if (!frozen) {
                if (element is MultiElement) {
                    var row = list.parent as ListBoxRow;

                    value->set_prop (DF_INDEX, row.get_index ().to_string ());
                    value_changed (value);
                } else {
                    Xml.Node* n = new Xml.Node (null, element.name);
                    n->add_child (value);

                    value_changed (n);
                }
            }
        }

        /** 
         * Determine the ordering of two properties based on the ordering given to them
         */
        private int sort_properties (ListBoxRow row1, ListBoxRow row2) {
            IPropertyNode e1 = row1.get_child () as IPropertyNode;
            IPropertyNode e2 = row2.get_child () as IPropertyNode;

            if (e1 == null) {
                return -1;
            } else if (e2 == null) {
                return 1;
            }

            return (int)(e1.sort_index > e2.sort_index)
                   - (int)(e1.sort_index < e2.sort_index);
        }

        private void on_add_request (Element elem) {
            add_item (elem, null);
        }

        private void add_item (Element elem, Xml.Node* node) {
            var list = new PropertyList (elem, node, builder, this);
            list.value_changed.connect (on_update_value_xml);
            add_child (list);
            list.set_index ((int)child_count - 1);
            if (header != null) {
                header.update ();
            }
            update_label ();

            if (!frozen) {
                Xml.Node* n = new Xml.Node (null, list.element.name);
                n->set_prop (DF_INDEX, (child_count - 1).to_string ());
                n->set_prop (DF_CREATED, null);
                value_changed (n);
            }

            show_all ();
        }

        private void clear_items () {
            var multi = element as MultiElement;
            for (uint i = child_count; i > 1 && i > multi.min_occurs; --i) {
                properties_list.remove (properties_list.get_row_at_index ((int)i - 1));
            }
            if (header != null) {
                header.update ();
            }
            update_label ();
        }

        private void on_type_changed (Element? e) {
            Widget w;
            while ((w = properties_list.get_row_at_index (0)) != null) {
                properties_list.remove (w);
            }

            element = e;

            build_from_element (e);

            if (!frozen && e != null) {
                Xml.Node* n = get_xml ();
                n->set_prop (DF_REPLACED, null);
                value_changed (n);
            }
        }

        private void build_from_element (Element elem, Xml.Node* node = null) {
            sort_index = elem.sort_index;
            expander.label = list_index == -1 ? elem.display_name : "%d: %s".printf (list_index, expander.label);

            var multi = elem as MultiElement;
            if (multi != null) {
                if (multi.min_occurs != multi.max_occurs || multi.min_occurs == 0) {
                    header = new MultiPropertyHeader (this, multi);
                    header.margin_start = 12;
                    header.add_request.connect (on_add_request);
                    header.clear_request.connect (clear_items);

                    expander.header_child = header;
                }

                if (node != null) {
                    for (Xml.Node* child = node->children; child != null; child = child->next) {
                        Element child_element = multi.get_choice_element (child->name);
                        if (child_element != null) {
                            add_item (child_element, child);
                        }
                    }
                }
            }

            foreach (Attribute attr in element.get_attributes ()) {
                PropertyType child_type = attr.prop_type;

                if (child_type != null) {
                    if (child_type is SimpleType || child_type is PrimitiveType) {
                        add_property (attr, node->get_prop (attr.name));
                    }
                }
            }
            foreach (Element e in element.get_elements ()) {
                Xml.Node* n = node;
                if (node != null && !(e is MultiElement)) {
                    for (n = node->children; n != null; n = n->next) {
                        if (n->name == e.name) {
                            break;
                        }
                    }
                }
                PropertyList list = new PropertyList (e, n, builder, this);
                add_child (list);
                list.value_changed.connect (on_update_value_xml);
                lists.add (list);
                if (multi != null) {
                    list.set_index ((int)child_count - 1);
                }
            }

            if (multi != null) {
                for (uint i = child_count; i < multi.min_occurs; ++i) {
                    PropertyList list = new PropertyList (null, null, builder, this);
                    add_child (list);
                    list.value_changed.connect (on_update_value_xml);
                    lists.add (list);
                    list.set_index ((int)child_count - 1);
                }
            }

            show_all ();
        }

        private void set_index (int index) {
            list_index = index;

            update_label ();
        }

        private void update_label () {
            if (element != null) {
                expander.label = list_index == -1 ? element.display_name : "%d: %s".printf (list_index, expander.label);
            } else {
                expander.label = list_index == -1 ? "" : list_index.to_string ();
            }

            if (is_collection) {
                var multi = (MultiElement)element;
                if (multi.min_occurs != 1 || multi.max_occurs != 1) {
                    uint count = child_count;
                    expander.label += " (%u item%s)".printf (count, count == 1 ? "" : "s");
                }
            }
        }

        private SizeGroup label_group = new SizeGroup (SizeGroupMode.BOTH);
        private SizeGroup control_group = new SizeGroup (SizeGroupMode.BOTH);
        private AdvancedExpander expander;
        private Box expander_content;
        private ListBox properties_list;
        private Gee.HashMap<string, PropertyEditor> widgets = new Gee.HashMap<string, PropertyEditor> ();
        private Gee.ArrayList<PropertyList> lists = new Gee.ArrayList<PropertyList> ();

        private MultiPropertyHeader header;
        private TypeSelectionHeader type_header;
        private PropertyList? parent_list = null;
        private int list_index = -1;
    }
}
