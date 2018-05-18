#include "resource/xmlutil.h"

#include "core/check.h"
#include "core/stringutil.h"

bool xml_property_read_bool(xmlNodePtr node, const char* name, bool* val) {
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
bool xml_property_read_int8(xmlNodePtr node, const char* name, int8* val) {
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
bool xml_property_read_uint8(xmlNodePtr node, const char* name, uint8* val) {
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
bool xml_property_read_int16(xmlNodePtr node, const char* name, int16* val) {
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
bool xml_property_read_uint16(xmlNodePtr node, const char* name, uint16* val) {
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
bool xml_property_read_int32(xmlNodePtr node, const char* name, int32* val) {
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
bool xml_property_read_uint32(xmlNodePtr node, const char* name, uint32* val) {
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
bool xml_property_read_float(xmlNodePtr node, const char* name, float* val) {
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
bool xml_property_read_double(xmlNodePtr node, const char* name, double* val) {
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
bool xml_property_read_vec2(xmlNodePtr node, const char* name, vec2* val) {
    int props_read = false;

    char* prop_name = saprintf("%s_x", name);
    props_read |= xml_property_read(node, prop_name, &val->x);
    sfree(prop_name);

    prop_name = saprintf("%s_y", name);
    props_read |= xml_property_read(node, prop_name, &val->y);
    sfree(prop_name);

    return props_read;
}
bool xml_property_read_vec3(xmlNodePtr node, const char* name, vec3* val) {
    bool props_read = false;

    props_read |= xml_property_read_vec2(node, name, &val->xy);

    char* prop_name = saprintf("%s_z", name);
    props_read |= xml_property_read(node, prop_name, &val->z);
    sfree(prop_name);

    return props_read;
}
bool xml_property_read_vec4(xmlNodePtr node, const char* name, vec4* val) {
    bool props_read = false;

    props_read |= xml_property_read_vec3(node, name, &val->xyz);

    char* prop_name = saprintf("%s_w", name);
    props_read |= xml_property_read(node, prop_name, &val->w);
    sfree(prop_name);

    return props_read;
}
bool xml_property_read_string(xmlNodePtr node, const char* name, char** val) {
    xmlChar* prop = xmlGetProp(node, (const xmlChar*)name);
    if(!prop)
        return false;

    *val = nstrdup((char*)prop);

    sfree(prop);

    return true;
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
    char* prop_name = saprintf("%s_x", name);

    bool ret = xml_property_write_float(writer, prop_name, val.x);
    sfree(prop_name);
    if(!ret) {
        return false;
    }

    prop_name = saprintf("%s_y", name);
    ret = xml_property_write_float(writer, prop_name, val.y);
    sfree(prop_name);

    return ret;
}
bool xml_property_write_vec3(xmlTextWriterPtr writer, const char* name, vec3 val) {
    if(!xml_property_write_vec2(writer, name, val.xy)) {
        return false;
    }

    char* prop_name = saprintf("%s_z", name);
    bool ret = xml_property_write_float(writer, prop_name, val.z);
    sfree(prop_name);

    return ret;
}
bool xml_property_write_vec4(xmlTextWriterPtr writer, const char* name, vec4 val) {
    if(!xml_property_write_vec3(writer, name, val.xyz)) {
        return false;
    }

    char* prop_name = saprintf("%s_w", name);
    bool ret = xml_property_write_float(writer, prop_name, val.w);
    sfree(prop_name);

    return ret;
}
bool xml_property_write_string(xmlTextWriterPtr writer, const char* name, char* val) {
    if(!val) {
        return false;
    }
    return xmlTextWriterWriteAttribute(writer, (xmlChar*)name, (xmlChar*)val) != -1;
}
