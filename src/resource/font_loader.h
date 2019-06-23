#ifndef DF_RESOURCE_FONT_LOADER
#define DF_RESOURCE_FONT_LOADER
#include "graphics/font.hd"

font load_font(const char* path, uint16 height);
font load_freetype_font(const char* path, uint16 height);

/** Load a font from image files
 *
 * This function uses a special XML file to define source files and glyph
 * metrics, see the 'text' demo for an example
 *
 * @param path The filepath pointing to the XML file
 */
font load_bitmap_font(const char* path);

#endif
