#ifndef DF_IO_UTIL_H
#define DF_IO_UTIL_H
#include "resource_def.h"
#include <stdbool.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Resource path get/set
//-----------------------------------------------------------------------------

/*!
 * This gets the path to the current base data directory.
 * By default, if this function is called with no data directory then the value
 * will be set to the directory containing the executable.
 */
const char* get_base_resource_path();

/*!
 * This constructs a string containing the path to a resource file.
 * Note that the user must free the returned string when finished.
 *
 * Usage:
 * resource_location is the extension to this directory. Setting this argument
 * to NULL will result in the file being loaded from the base resource
 * directory.
 * resource_name is the actual name of the file.
 *
 * Example 1: resources/data/foo.bar
 * resource_location: "data"
 * resource_name: "foo.bar"
 *
 * Example 2: resources/foo.bar
 * resource_location: NULL
 * resource_name: "foo.bar"
 */
char* construct_extended_resource_path(resource_pair);

/*!
 * This sets the path of the base data directory.
 */
void set_resource_path(const char* path);

/*!
 * This sets the path of the base data directory.
 * The path argument is treated as an extension to the directory containing the
 * executable.
 */
void set_resource_path_relative(const char* path);

//-----------------------------------------------------------------------------
// Resource loading code
//-----------------------------------------------------------------------------

/*!
 * Opens a file from the resource path and extension.
 * See get_extended_resource_path(io_util.h) for usage details
 *
 * appending a ! to the end of mode will cause this function to try swapping an
 * initial r with w if the file cannot be loaded.
 */
FILE* load_resource_file(resource_pair, const char* mode);

/*!
 * Loads a file in the resource directory to a new buffer.
 * The user must handle freeing the returned buffer's memory.
 * See get_extended_resource_path(io_util.h) for usage details
 */
unsigned char* load_resource_to_buffer(resource_pair);

/*!
 * This funcion loads a string from a file.
 * The string should be formatted as such: <length(2 bytes)><string data>
 * The file pointer will be advanced to the end of the string.
 */
char* read_string_from_file(FILE* file);

/*!
 * This funcion writes a string ito a file.
 * The string will be formatted as such: <length(2 bytes)><string data>
 * The file pointer will be advanced to the end of the string.
 */
void write_string_to_file(FILE* file, const char* str);

//-----------------------------------------------------------------------------
// Other code
//-----------------------------------------------------------------------------

/*!
 * This checks if a resource doesn't exist. If it does, it alters the name by
 * incrementing it until it finds a free id.
 * E.g. File might try File (1), then File (2), then File (3), etc.
 *
 * See get_extended_resource_path(io_util.h) for additional usage details
 */
char* get_unique_resource_name(resource_pair);

/*!
 * Checks to see if the directory at path exists. If not, the directory is
 * created.
 * Returns true if a new directory was created, and false if it existed
 * already.
 */
bool ensure_directory(const char* path);

/*!
 * Returns a string with file_name's extension changed to file_extension. If
 * file_extension is NULL, file_name's extension is removed.
 *
 * Note that the resulting string must be freed by the user.
 */
char* swap_extension(resource_pair);

/*!
 * Returns a substring with a file's extension.
 * This substring doesn't need freeing; it's a pointer to the original string.
 */
const char* get_extension(const char* file_name);

/*!
 * Loads a resource of undefined type, and returns a void pointer.
 * This shouldn't generally be called manually, as it is used by the resource
 * system.
 */
void* load_anonymous_resource(resource_pair);

/*!
 * Tests to see if two pairs of resource data refer to the same path
 */
bool resource_eq(resource_pair, const char* resource2_location, const char* resource2_name);

/*!
 * Test if a given resource exists
 */
bool resource_exists(resource_pair);

/*!
 * Converts an absolute path to a relative content path, or NULL if it doesn't
 * point to the content directory.
 */
char* path_to_content_path(const char* path);

#endif
