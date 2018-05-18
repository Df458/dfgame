#ifndef DF_RESOURCE_XML_UTIL
#define DF_RESOURCE_XML_UTIL

#include "math/vector.h"
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>

bool xml_property_read_bool(xmlNodePtr node, const char* name, bool* val);
bool xml_property_read_int8(xmlNodePtr node, const char* name, int8* val);
bool xml_property_read_uint8(xmlNodePtr node, const char* name, uint8* val);
bool xml_property_read_int16(xmlNodePtr node, const char* name, int16* val);
bool xml_property_read_uint16(xmlNodePtr node, const char* name, uint16* val);
bool xml_property_read_int32(xmlNodePtr node, const char* name, int32* val);
bool xml_property_read_uint32(xmlNodePtr node, const char* name, uint32* val);
bool xml_property_read_float(xmlNodePtr node, const char* name, float* val);
bool xml_property_read_double(xmlNodePtr node, const char* name, double* val);
bool xml_property_read_vec2(xmlNodePtr node, const char* name, vec2* val);
bool xml_property_read_vec3(xmlNodePtr node, const char* name, vec3* val);
bool xml_property_read_vec4(xmlNodePtr node, const char* name, vec4* val);
bool xml_property_read_string(xmlNodePtr node, const char* name, char** val);
                                                
#define xml_property_read(node, name, val) _Generic(val,\
    bool*: xml_property_read_bool,\
    int8*: xml_property_read_int8,\
    uint8*: xml_property_read_uint8,\
    int16*: xml_property_read_int16,\
    uint16*: xml_property_read_uint16,\
    int32*: xml_property_read_int32,\
    uint32*: xml_property_read_uint32,\
    float*: xml_property_read_float,\
    double*: xml_property_read_double,\
    vec2*: xml_property_read_vec2,\
    vec3*: xml_property_read_vec3,\
    vec4*: xml_property_read_vec4,\
    char**: xml_property_read_string\
)(node, name, val)

bool xml_property_write_bool(xmlTextWriterPtr writer, const char* name, bool val);
bool xml_property_write_int8(xmlTextWriterPtr writer, const char* name, int8 val);
bool xml_property_write_uint8(xmlTextWriterPtr writer, const char* name, uint8 val);
bool xml_property_write_int16(xmlTextWriterPtr writer, const char* name, int16 val);
bool xml_property_write_uint16(xmlTextWriterPtr writer, const char* name, uint16 val);
bool xml_property_write_int32(xmlTextWriterPtr writer, const char* name, int32 val);
bool xml_property_write_uint32(xmlTextWriterPtr writer, const char* name, uint32 val);
bool xml_property_write_float(xmlTextWriterPtr writer, const char* name, float val);
bool xml_property_write_double(xmlTextWriterPtr writer, const char* name, double val);
bool xml_property_write_vec2(xmlTextWriterPtr writer, const char* name, vec2 val);
bool xml_property_write_vec3(xmlTextWriterPtr writer, const char* name, vec3 val);
bool xml_property_write_vec4(xmlTextWriterPtr writer, const char* name, vec4 val);
bool xml_property_write_string(xmlTextWriterPtr writer, const char* name, char* val);
                                                
#define xml_property_write(writer, name, val) _Generic(val,\
    bool: xml_property_write_bool,\
    int8: xml_property_write_int8,\
    uint8: xml_property_write_uint8,\
    int16: xml_property_write_int16,\
    uint16: xml_property_write_uint16,\
    int32: xml_property_write_int32,\
    uint32: xml_property_write_uint32,\
    float: xml_property_write_float,\
    double: xml_property_write_double,\
    vec2: xml_property_write_vec2,\
    vec3: xml_property_write_vec3,\
    vec4: xml_property_write_vec4,\
    char*: xml_property_write_string\
)(writer, name, val)

#endif
