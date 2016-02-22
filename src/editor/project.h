#ifndef PROJECT_H
#define PROJECT_H
#include <stdbool.h>

typedef struct project project;

bool create_project(const char* path);
bool load_project(const char* path);

const char* get_project_name();
const char* get_project_path();

#endif
