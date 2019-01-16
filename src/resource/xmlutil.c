#include "resource/xmlutil.h"

#include "core/check.h"
#include "core/stringutil.h"

// Finds the next sibling of node (including itself) that has a given name
xmlNodePtr xml_match_name(const xmlNodePtr node, const char* name) {
    for(xmlNodePtr cursor = node; cursor; cursor = cursor->next)
        if(cursor->type == XML_ELEMENT_NODE && !xmlStrcmp(cursor->name, (const xmlChar*)name))
            return cursor;

    return NULL;
}

bool xml_property_read_bool(const xmlNodePtr node, const char* name, bool* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    // TODO: Compare should be case-insensitive
    if(!strcmp((char*)prop, "true") || !strcmp((char*)prop, "True")) {
        *val = true;
    } else if(!strcmp((char*)prop, "false") || !strcmp((char*)prop, "False")) {
        *val = false;
    } else {
        error("Xml property \"%s\" (\"%s\") can't be stored as a bool because its value is neither true nor false", name, prop);

        return false;
    }

    sfree(prop);

    return true;
}
bool xml_property_read_int8(const xmlNodePtr node, const char* name, int8* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    if(check_error(aisi((char*)prop), "Xml property \"%s\" (\"%s\") is not an integer", name, prop)) {
        sfree(prop);
        return false;
    }

    int64 pval = atoi((char*)prop);

    sfree(prop);

    if(check_error(pval <= INT8_MAX, "Xml property \"%s\" (%d) is too large to store in an int8", name, pval)) {
        return false;
    }
    *val = pval;

    return true;
}
bool xml_property_read_uint8(const xmlNodePtr node, const char* name, uint8* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    if(check_error(aisi((char*)prop), "Xml property \"%s\" (\"%s\") is not an integer", name, prop)) {
        sfree(prop);
        return false;
    }

    int64 pval = atoi((char*)prop);

    sfree(prop);

    if(check_error(pval >= 0, "Xml property \"%s\" (%d) is negative, but is being added to an unsigned field", name, pval)) {
        return false;
    }
    if(check_error(pval <= UINT8_MAX, "Xml property \"%s\" (%d) is too large to store in a uint8", name, pval)) {
        return false;
    }
    *val = pval;

    return true;
}
bool xml_property_read_int16(const xmlNodePtr node, const char* name, int16* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    if(check_error(aisi((char*)prop), "Xml property \"%s\" (\"%s\") is not an integer", name, prop)) {
        sfree(prop);
        return false;
    }

    int64 pval = atoi((char*)prop);

    sfree(prop);

    if(check_error(pval <= INT16_MAX, "Xml property \"%s\" (%d) is too large to store in an int16", name, pval)) {
        return false;
    }
    *val = pval;

    return true;
}
bool xml_property_read_uint16(const xmlNodePtr node, const char* name, uint16* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    if(check_error(aisi((char*)prop), "Xml property \"%s\" (\"%s\") is not an integer", name, prop)) {
        sfree(prop);
        return false;
    }

    int64 pval = atoi((char*)prop);

    sfree(prop);

    if(check_error(pval >= 0, "Xml property \"%s\" (%d) is negative, but is being added to an unsigned field", name, pval)) {
        return false;
    }
    if(check_error(pval <= UINT16_MAX, "Xml property \"%s\" (%d) is too large to store in a uint16", name, pval)) {
        return false;
    }
    *val = pval;

    return true;
}
bool xml_property_read_int32(const xmlNodePtr node, const char* name, int32* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    if(check_error(aisi((char*)prop), "Xml property \"%s\" (\"%s\") is not an integer", name, prop)) {
        sfree(prop);
        return false;
    }

    int64 pval = atoi((char*)prop);

    sfree(prop);

    if(check_error(pval <= INT32_MAX, "Xml property \"%s\" (%d) is too large to store in an int32", name, pval)) {
        return false;
    }
    *val = pval;

    return true;
}
bool xml_property_read_uint32(const xmlNodePtr node, const char* name, uint32* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    if(check_error(aisi((char*)prop), "Xml property \"%s\" (\"%s\") is not an integer", name, prop)) {
        sfree(prop);
        return false;
    }

    int64 pval = atoi((char*)prop);

    sfree(prop);

    if(check_error(pval >= 0, "Xml property \"%s\" (%d) is negative, but is being added to an unsigned field", name, pval)) {
        return false;
    }
    if(check_error(pval <= UINT32_MAX, "Xml property \"%s\" (%d) is too large to store in a uint32", name, pval)) {
        return false;
    }
    *val = pval;

    return true;
}
bool xml_property_read_float(const xmlNodePtr node, const char* name, float* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop)
        return false;

    if(check_error(aisf((char*)prop), "Xml property \"%s\" (\"%s\") is not a floating-point number", name, prop)) {
        sfree(prop);
        return false;
    }

    *val = atof((char*)prop);
    sfree(prop);

    return true;
}
bool xml_property_read_double(const xmlNodePtr node, const char* name, double* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop)
        return false;

    if(check_error(aisf((char*)prop), "Xml property \"%s\" (\"%s\") is not a floating-point number", name, prop)) {
        sfree(prop);
        return false;
    }

    *val = atof((char*)prop);

    sfree(prop);

    return true;
}
bool xml_property_read_vec2(const xmlNodePtr node, const char* name, vec2* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    uint16 count;
    char** tokens = nstrsplit((char*)prop, " \t", &count);

    bool props_read = count == 2;

    for(uint16 i = 0; props_read && i < count; ++i) {
        if(!aisf(tokens[i])) {
            props_read = false;
            break;
        }

        val->data[i] = atof(tokens[i]);
    }

    if(tokens != NULL) {
        for(uint16 i = 0; i < count; ++i) {
            sfree(tokens[i]);
        }
        sfree(tokens);
    }
    sfree(prop);

    return props_read;
}
bool xml_property_read_vec3(const xmlNodePtr node, const char* name, vec3* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    uint16 count;
    char** tokens = nstrsplit((char*)prop, " \t", &count);

    bool props_read = count == 3;

    for(uint16 i = 0; props_read && i < count; ++i) {
        if(!aisf(tokens[i])) {
            props_read = false;
            break;
        }

        val->data[i] = atof(tokens[i]);
    }

    if(tokens != NULL) {
        for(uint16 i = 0; i < count; ++i) {
            sfree(tokens[i]);
        }
        sfree(tokens);
    }
    sfree(prop);

    return props_read;
}
bool xml_property_read_vec4(const xmlNodePtr node, const char* name, vec4* val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop) {
        return false;
    }

    uint16 count;
    char** tokens = nstrsplit((char*)prop, " \t", &count);

    bool props_read = count == 4;

    for(uint16 i = 0; props_read && i < count; ++i) {
        if(!aisf(tokens[i])) {
            props_read = false;
            break;
        }

        val->data[i] = atof(tokens[i]);
    }

    if(tokens != NULL) {
        for(uint16 i = 0; i < count; ++i) {
            sfree(tokens[i]);
        }
        sfree(tokens);
    }
    sfree(prop);

    return props_read;
}
bool xml_property_read_string(const xmlNodePtr node, const char* name, char** val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop)
        return false;

    *val = nstrdup((char*)prop);

    sfree(prop);

    return true;
}

bool xml_property_read_color3(const xmlNodePtr node, const char* name, vec3* val) {
    uint8 r, g, b;
    bool props_read = false;

    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop || strlen((char*)prop) != 7) {
        return false;
    }

    uint8 count = sscanf((char*)prop, "#%02hhx%02hhx%02hhx", &r, &g, &b);
    if(count == 3) {
        val->r = r / 255.0f;
        val->g = g / 255.0f;
        val->b = b / 255.0f;

        props_read = true;
    }

    sfree(prop);

    return props_read;
}
bool xml_property_read_color4(const xmlNodePtr node, const char* name, vec4* val) {
    uint8 r, g, b, a;
    bool props_read = false;

    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop || strlen((char*)prop) != 9) {
        return false;
    }

    uint8 count = sscanf((char*)prop, "#%02hhx%02hhx%02hhx%02hhx", &r, &g, &b, &a);
    if(count == 4) {
        val->r = r / 255.0f;
        val->g = g / 255.0f;
        val->b = b / 255.0f;
        val->a = a / 255.0f;

        props_read = true;
    }

    sfree(prop);

    return props_read;
}

bool xml_property_write_bool(xmlTextWriterPtr writer, const char* name, bool val) {
    return xmlTextWriterWriteAttribute(writer, (xmlChar*)name, val ? (xmlChar*)"true" : (xmlChar*)"false") != -1;
}
bool xml_property_write_int8(xmlTextWriterPtr writer, const char* name, int8 val) {
    char* data = saprintf("%hhd", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_uint8(xmlTextWriterPtr writer, const char* name, uint8 val) {
    char* data = saprintf("%hhu", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_int16(xmlTextWriterPtr writer, const char* name, int16 val) {
    char* data = saprintf("%hd", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_uint16(xmlTextWriterPtr writer, const char* name, uint16 val) {
    char* data = saprintf("%hu", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_int32(xmlTextWriterPtr writer, const char* name, int32 val) {
    char* data = saprintf("%d", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_uint32(xmlTextWriterPtr writer, const char* name, uint32 val) {
    char* data = saprintf("%u", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_float(xmlTextWriterPtr writer, const char* name, float val) {
    char* data = saprintf("%f", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_double(xmlTextWriterPtr writer, const char* name, double val) {
    char* data = saprintf("%f", val);
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_vec2(xmlTextWriterPtr writer, const char* name, vec2 val) {
    char* data = saprintf("%f %f", vec2_decomp(val));
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_vec3(xmlTextWriterPtr writer, const char* name, vec3 val) {
    char* data = saprintf("%f %f %f", vec3_decomp(val));
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_vec4(xmlTextWriterPtr writer, const char* name, vec4 val) {
    char* data = saprintf("%f %f %f %f", vec4_decomp(val));
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_string(xmlTextWriterPtr writer, const char* name, const char* val) {
    if(!val) {
        return false;
    }
    return xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)val) != -1;
}

bool xml_property_write_color3(xmlTextWriterPtr writer, const char* name, vec3 val) {
    char* data = saprintf("#%02hhx%02hhx%02hhx", (int)(val.r * 255), (int)(val.g * 255), (int)(val.b * 255));
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
bool xml_property_write_color4(xmlTextWriterPtr writer, const char* name, vec4 val) {
    char* data = saprintf("#%02hhx%02hhx%02hhx%02hhx", (int)(val.r * 255), (int)(val.g * 255), (int)(val.b * 255), (int)(val.a * 255));
    int ret = xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)data);
    sfree(data);

    return ret != -1;
}
