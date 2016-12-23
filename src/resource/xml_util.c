#include "xml_util.h"
#include "io_util.h"
#include "util.h"

xmlDocPtr load_resource_to_xml(resource_pair)
{
    char* path = construct_extended_resource_path(resource_location, resource_name);
    xmlDocPtr doc = xmlReadFile(path, NULL, 0);
    free(path);
    return doc;
}

xmlTextWriterPtr create_xml_resource(resource_pair)
{
    unsigned char* path = (unsigned char*)construct_extended_resource_path(resource_location, resource_name);
    xmlChar* uri = xmlPathToURI(path);
    xmlTextWriterPtr writer = xmlNewTextWriterFilename((char*)uri, 0);
    if(!writer)
        warn("Failed to create XML writer\n");
    free(uri);
    free(path);
    return writer;
}

