#include "xml.h"
#include "resource/xmlutil.h"
#include "log/log.h"
#include <libxml/parser.h>

static const char test_file_1[] = "<root small_int=\"1\" big_int=\"1234567\" negative_int=\"-40\" vec2=\"12.5 0.25\" vec3=\"1.25 -35.7 45.01\" vec4=\"1 -0.15 0 6.250\" vec5=\"1 -0.15 0 6.250 7\" float=\"1.25\" string=\"lorem ipsum\" bool=\"true\" Bool=\"False\" color3=\"#ff00ff\" Color3=\"#00FFFF\" color4=\"#ffffffff\" Color4=\"#FFFF0000\"/>";
static const char test_file_2[] = "<root><child v=\"1\"/><match v=\"2\"/><child v=\"3\"/><child v=\"4\"/><match v=\"5\"/></root>";

static xmlDocPtr test_doc = NULL;
static xmlDocPtr test_doc_2 = NULL;

int test_xml_init() {
    test_doc = xmlReadMemory(test_file_1, strlen(test_file_1), "test.xml", NULL, 0);
    test_doc_2 = xmlReadMemory(test_file_2, strlen(test_file_2), "test.xml", NULL, 0);

    return 0;
}
int test_xml_cleanup() {
    if(test_doc) {
        xmlFreeDoc(test_doc);
        test_doc = NULL;
    }
    if(test_doc_2) {
        xmlFreeDoc(test_doc_2);
        test_doc_2 = NULL;
    }
    return 0;
}

void test_xml_build_tests(CU_pSuite suite) {
    CU_ADD_TEST(suite, test_xml_read);
    CU_ADD_TEST(suite, test_xml_match);
}

void test_xml_read() {
    xmlNodePtr root = xmlDocGetRootElement(test_doc);
    bool test_bool = false;
    uint8 test_uint8 = 0;
    int8 test_int8 = 0;
    uint16 test_uint16 = 0;
    int16 test_int16 = 0;
    uint32 test_uint32 = 0;
    int32 test_int32 = 0;
    float test_float = 0;
    double test_double = 0;
    vec2 test_vec2 = vec2_zero;
    vec3 test_vec3 = vec3_zero;
    vec4 test_vec4 = vec4_zero;
    char* test_string = NULL;

    // Bool tests
    CU_ASSERT_TRUE(xml_property_read(root, "bool", &test_bool));
    CU_ASSERT_TRUE(test_bool);
    CU_ASSERT_TRUE(xml_property_read(root, "Bool", &test_bool));
    CU_ASSERT_FALSE(test_bool);
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_bool));

    // Int tests
    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_uint8));
    CU_ASSERT_EQUAL(test_uint8, 1);
    CU_ASSERT_FALSE(xml_property_read(root, "big_int", &test_uint8));
    CU_ASSERT_FALSE(xml_property_read(root, "negative_int", &test_uint8));
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_uint8));

    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_int8));
    CU_ASSERT_EQUAL(test_int8, 1);
    CU_ASSERT_FALSE(xml_property_read(root, "big_int", &test_int8));
    CU_ASSERT_TRUE(xml_property_read(root, "negative_int", &test_int8));
    CU_ASSERT_EQUAL(test_int8, -40);
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_int8));

    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_uint16));
    CU_ASSERT_EQUAL(test_uint16, 1);
    CU_ASSERT_FALSE(xml_property_read(root, "big_int", &test_uint16));
    CU_ASSERT_FALSE(xml_property_read(root, "negative_int", &test_uint16));
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_uint16));

    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_int16));
    CU_ASSERT_EQUAL(test_int16, 1);
    CU_ASSERT_FALSE(xml_property_read(root, "big_int", &test_int16));
    CU_ASSERT_TRUE(xml_property_read(root, "negative_int", &test_int16));
    CU_ASSERT_EQUAL(test_int16, -40);
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_int16));

    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_uint32));
    CU_ASSERT_EQUAL(test_uint32, 1);
    CU_ASSERT_TRUE(xml_property_read(root, "big_int", &test_uint32));
    CU_ASSERT_EQUAL(test_uint32, 1234567);
    CU_ASSERT_FALSE(xml_property_read(root, "negative_int", &test_uint32));
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_uint32));

    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_int32));
    CU_ASSERT_EQUAL(test_int32, 1);
    CU_ASSERT_TRUE(xml_property_read(root, "big_int", &test_int32));
    CU_ASSERT_EQUAL(test_int32, 1234567);
    CU_ASSERT_TRUE(xml_property_read(root, "negative_int", &test_int32));
    CU_ASSERT_EQUAL(test_int32, -40);
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_int32));

    // Float tests
    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_float));
    CU_ASSERT_EQUAL(test_float, 1);
    CU_ASSERT_TRUE(xml_property_read(root, "float", &test_float));
    CU_ASSERT_EQUAL(test_float, 1.25f);
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_float));

    CU_ASSERT_TRUE(xml_property_read(root, "small_int", &test_double));
    CU_ASSERT_EQUAL(test_double, 1);
    CU_ASSERT_TRUE(xml_property_read(root, "float", &test_double));
    CU_ASSERT_EQUAL(test_double, 1.25f);
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_double));


    // Vector tests
    CU_ASSERT_TRUE(xml_property_read(root, "vec2", &test_vec2));
    CU_ASSERT_EQUAL(test_vec2.x, 12.5f);
    CU_ASSERT_EQUAL(test_vec2.y, 0.25f);
    CU_ASSERT_FALSE(xml_property_read(root, "vec3", &test_vec2));
    CU_ASSERT_FALSE(xml_property_read(root, "vec4", &test_vec2));
    CU_ASSERT_FALSE(xml_property_read(root, "vec5", &test_vec2));
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_vec2));

    CU_ASSERT_TRUE(xml_property_read(root, "vec3", &test_vec3));
    CU_ASSERT_EQUAL(test_vec3.x, 1.25f);
    CU_ASSERT_EQUAL(test_vec3.y, -35.7f);
    CU_ASSERT_EQUAL(test_vec3.z, 45.01f);
    CU_ASSERT_FALSE(xml_property_read(root, "vec2", &test_vec3));
    CU_ASSERT_FALSE(xml_property_read(root, "vec4", &test_vec3));
    CU_ASSERT_FALSE(xml_property_read(root, "vec5", &test_vec3));
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_vec3));

    CU_ASSERT_TRUE(xml_property_read(root, "vec4", &test_vec4));
    CU_ASSERT_EQUAL(test_vec4.x, 1.0f);
    CU_ASSERT_EQUAL(test_vec4.y, -0.15f);
    CU_ASSERT_EQUAL(test_vec4.z, 0.0f);
    CU_ASSERT_EQUAL(test_vec4.w, 6.25f);
    CU_ASSERT_FALSE(xml_property_read(root, "vec2", &test_vec4));
    CU_ASSERT_FALSE(xml_property_read(root, "vec3", &test_vec4));
    CU_ASSERT_FALSE(xml_property_read(root, "vec5", &test_vec4));
    CU_ASSERT_FALSE(xml_property_read(root, "string", &test_vec4));

    // Color tests
    CU_ASSERT_TRUE(xml_property_read_color(root, "color3", &test_vec3));
    CU_ASSERT_EQUAL(test_vec3.r, 1.0f);
    CU_ASSERT_EQUAL(test_vec3.g, 0.0f);
    CU_ASSERT_EQUAL(test_vec3.b, 1.0f);
    CU_ASSERT_TRUE(xml_property_read_color(root, "Color3", &test_vec3));
    CU_ASSERT_EQUAL(test_vec3.r, 0.0f);
    CU_ASSERT_EQUAL(test_vec3.g, 1.0f);
    CU_ASSERT_EQUAL(test_vec3.b, 1.0f);
    CU_ASSERT_FALSE(xml_property_read_color(root, "color4", &test_vec3));
    CU_ASSERT_FALSE(xml_property_read_color(root, "Color4", &test_vec3));

    CU_ASSERT_TRUE(xml_property_read_color(root, "color4", &test_vec4));
    CU_ASSERT_EQUAL(test_vec4.r, 1.0f);
    CU_ASSERT_EQUAL(test_vec4.g, 1.0f);
    CU_ASSERT_EQUAL(test_vec4.b, 1.0f);
    CU_ASSERT_EQUAL(test_vec4.a, 1.0f);
    CU_ASSERT_TRUE(xml_property_read_color(root, "Color4", &test_vec4));
    CU_ASSERT_EQUAL(test_vec4.r, 1.0f);
    CU_ASSERT_EQUAL(test_vec4.g, 1.0f);
    CU_ASSERT_EQUAL(test_vec4.b, 0.0f);
    CU_ASSERT_EQUAL(test_vec4.a, 0.0f);
    CU_ASSERT_FALSE(xml_property_read_color(root, "color3", &test_vec4));
    CU_ASSERT_FALSE(xml_property_read_color(root, "Color3", &test_vec4));


    // String tests
    CU_ASSERT_TRUE(xml_property_read(root, "string", &test_string));
    CU_ASSERT_EQUAL(strcmp(test_string, "lorem ipsum"), 0);
    sfree(test_string);
}

void test_xml_match() {
    xmlNodePtr root = xmlDocGetRootElement(test_doc_2);

    uint8 test_val = 0;

    xmlNodePtr match = xml_match_name(root->children, "match");
    CU_ASSERT_TRUE(xml_property_read(match, "v", &test_val));
    CU_ASSERT_EQUAL(test_val, 2);
    match = xml_match_name(match->next, "match");
    CU_ASSERT_TRUE(xml_property_read(match, "v", &test_val));
    CU_ASSERT_EQUAL(test_val, 5);

    match = xml_match_name(root->children, "child");
    CU_ASSERT_TRUE(xml_property_read(match, "v", &test_val));
    CU_ASSERT_EQUAL(test_val, 1);
    match = xml_match_name(match->next, "child");
    CU_ASSERT_TRUE(xml_property_read(match, "v", &test_val));
    CU_ASSERT_EQUAL(test_val, 3);
    match = xml_match_name(match->next, "child");
    CU_ASSERT_TRUE(xml_property_read(match, "v", &test_val));
    CU_ASSERT_EQUAL(test_val, 4);
}
