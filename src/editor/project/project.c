#include "asset_registry.h"
#include "xml_util.h"
#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define PROJECT_FILE_VERSION "0.1.0"

char* project_name;

bool create_project(const char* path)
{
    // Check to see if the project directory exists already
    if(!ensure_directory(path))
        return false;

    // Create subdirectories and set the resource path
    char* content_path = calloc(strlen(path) + 8 + 1, sizeof(char));
    strcpy(content_path, path);
    char* log_path = strcat(content_path, "/logs");
    mkdir(log_path, 0750);
    content_path = strcat(content_path, "/content");
    mkdir(content_path, 0750);
    set_resource_path(content_path);
    free(content_path);

    const char* title_ptr = strrchr(path, '/') + 1;
    project_name = calloc(strlen(title_ptr) + 1, sizeof(char));
    strncpy(project_name, title_ptr, strlen(title_ptr));

    char* project_fname = calloc(strlen(project_name) + 5, sizeof(char));
    strcpy(project_fname, project_name);
    // TODO: Make this configurable
    strcat(project_fname, ".hal");

    xmlTextWriterPtr writer = create_xml_resource("..", project_fname);
    free(project_fname);
    xmlTextWriterSetIndent(writer, 2);

    xmlTextWriterStartDocument(writer, NULL, NULL, NULL);
    xmlTextWriterStartElement(writer, "project");
    xmlTextWriterWriteAttribute(writer, "version", PROJECT_FILE_VERSION);
    //--------------------------------------------------------------------------
    xmlTextWriterStartElement(writer, "start");
    // TODO: Write initial map and start script info here
    xmlTextWriterEndElement(writer);
    xmlTextWriterStartElement(writer, "settings");
    // TODO: Write settings here
    xmlTextWriterEndElement(writer);
    //--------------------------------------------------------------------------
    xmlTextWriterEndElement(writer);
    xmlTextWriterEndDocument(writer);

    xmlFreeTextWriter(writer);

    create_resource_definition_file();
    // TODO: Set the log file?

    return true;
}

bool load_project(const char* path)
{
    const char* title_ptr = strrchr(path, '/') + 1;
    project_name = calloc(strlen(title_ptr) - 4 + 1, sizeof(char));
    strncpy(project_name, title_ptr, strlen(title_ptr) - 4);

    char* content_path = calloc(strlen(path) - strlen(title_ptr) + 8 + 1, sizeof(char));
    strncpy(content_path, path, strlen(path) - strlen(title_ptr));
    strcat(content_path, "content");
    set_resource_path(content_path);
    resources_init(load_anonymous_resource);
    // TODO: Set the log file
    return true;
}

const char* get_project_name()
{
    return project_name;
}
