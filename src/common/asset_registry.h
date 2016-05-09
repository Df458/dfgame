#ifndef DF_ASSET_REGISTRY
#define DF_ASSET_REGISTRY

#include "io_util.h"
#include <inttypes.h>
#include <stdbool.h>

// The number of empty slots to add when the resource list is filled
#define RESOURCE_BLOCK_COUNT 100

typedef void* (*resource_loader)(resource_pair);

/*!
 * Initializes the resource system
 * loader should be a function that determines what a resource is, loads it,
 * and returns a void pointer to the result. Note that the data will be
 * managed, and should not be freed by other parts of the application.
 */
bool resources_init(resource_loader loader);

/*!
 * Cleans up the resource system
 */
void resources_cleanup();

/*!
 * Creates a new empty resource definition file
 */
bool create_resource_definition_file();

/*!
 * Returns the ID of a registered resource. If the resource is not registered,
 * a new ID is returned.
 */
uint32_t get_id_from_resource(resource_pair, bool should_create);

/*!
 * Returns the data from the resource with the specified id, if it's loaded.
 * If the resource is not loaded, the load function specified by resources_init
 * is called.
 * This data is managed, and should not be deleted, but it is intentionally
 * left non-const to allow modifications.
 */
void*    get_data_from_id(uint32_t id);

/*!
 * Returns the name of the resource with the specified id. The returned data
 * must be freed by the caller.
 */
char*    get_name_from_id(uint32_t id, bool force_invalid);

/*!
 * Returns the path of the resource with the specified id. The returned data
 * must be freed by the caller.
 */
char*    get_path_from_id(uint32_t id, bool force_invalid);

/*!
 * Returns the data from the resource with the specified location, if it's loaded.
 * If the resource is not loaded, the load function specified by resources_init
 * is called.
 * This data is managed, and should not be deleted, but it is intentionally
 * left non-const to allow modifications.
 */
void*    get_data_from_resource(resource_pair);

/*!
 * Changes a registered resource's location.
 */
bool     move_resource(const char* prev_location, const char* prev_name, const char* next_location, const char* next_name);

/*!
 * Unregisters a resource, returning the data. This data will be disregarded at
 * this point, and it is the user's responsibility to free it.
 */
void*    delete_resource(resource_pair);

/*!
 * Changes a registered resource's location.
 */
bool     move_id(uint32_t id, const char* next_location, const char* next_name);

/*!
 * Unregisters a resource, returning any data it currently contains. This data
 * will be disregarded at this point, and it is the user's responsibility to
 * free it.
 */
void*    delete_id(uint32_t id);

/*!
 * Returns the number of failed resources
 */
uint32_t get_failed_count();

/*!
 * Returns an array with the ids of all failed resources.
 * Note that the calling function is responsible for freeing the data.
 */
uint32_t* get_failed_ids();

#endif
