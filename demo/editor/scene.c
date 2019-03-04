#include "scene.h"

#include "resource/xmlutil.h"
#include "triangle.h"

#include <libxml/tree.h>
#include <locale.h>

scene* scene_init() {
    scene* s = msalloc(struct scene);
    s->tris = array_mnew_ordered(triangle*, 4);
    triangle* t = triangle_init();
    array_add(s->tris, t);

    return s;
}
void scene_free(scene* s) {
    array_foreach(s->tris, it) {
        triangle_free(array_iter_data(it, triangle*));
    }
    array_free(s->tris);
    sfree(s);
}
void scene_draw(scene* s) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    array_foreach(s->tris, it) {
        triangle_draw(array_iter_data(it, triangle*));
    }
}
void scene_load(scene* s, xmlNodePtr data) {
    setlocale(LC_NUMERIC, "C");

    for(xmlNodePtr tri = xml_match_name(data->children, "triangle"); tri != NULL; tri = xml_match_name(tri->next, "triangle")) {
        container_index index;
        if (xml_property_read(tri, "index", &index)) {
            if(xmlHasProp(tri, (xmlChar*)"created")) {
                triangle* t = triangle_init();
                array_insert(s->tris, &t, index);
            } else if(xmlHasProp(tri, (xmlChar*)"created")) {
                array_remove_at(s->tris, index);
            } else {
                triangle* t = *(triangle**)array_get(s->tris, index);
                triangle_load(t, tri);
            }
        }
    }
}
void scene_reset(scene* s) {
    array_rforeach(s->tris, it) {
        triangle_free(array_iter_data(it, triangle*));
        array_remove_iter(s->tris, &it);
    }

    triangle* t = triangle_init();
    array_add(s->tris, t);
}
char* scene_save(scene* s) {
    xmlBuffer buf = {0};
    xmlTextWriter* writer = xmlNewTextWriterMemory(&buf, 0);

    setlocale(LC_NUMERIC, "C");
    xmlTextWriterStartDocument(writer, NULL, "ISO-8859-1", NULL);
    xmlTextWriterStartElement(writer, (xmlChar*)"scene");

    array_foreach(s->tris, it) {
        triangle_save(array_iter_data(it, triangle*), writer);
    }

    xmlTextWriterEndElement(writer);
    xmlTextWriterEndDocument(writer);

    xmlFreeTextWriter(writer);
    return (char*)buf.content;
}
