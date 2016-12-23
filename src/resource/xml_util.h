#ifndef DF_XML_UTIL_H
#define DF_XML_UTIL_H
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/uri.h>
#include <libxml/xmlwriter.h>
#include "resource_def.h"

/*!
 * This loads an XML file from the resource directory.
 * See get_extended_resource_path(io_util.h) for usage details
 */
xmlDocPtr load_resource_to_xml(resource_pair);

/*!
 * This creates a new XML file in the resource directory.
 * See get_extended_resource_path(io_util.h) for usage details
 */
xmlTextWriterPtr create_xml_resource(resource_pair);

// TODO: Add reading/writing util functions
#endif
