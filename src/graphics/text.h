/**
 * @file text.h
 * @author Hugues Ross
 * @date 02 February 2019
 * @brief Provides functions for creating and rendering text
 */

#ifndef DF_GRAPHICS_TEXT
#define DF_GRAPHICS_TEXT

#include "graphics/text.hd"
#include "graphics/font.hd"
#include "graphics/mesh.hd"
#include "graphics/shader.hd"
#include "math/alignment.h"
#include "math/matrix.hd"

/** @brief Represents a type of text wrapping
 *
 * These values are used in text_get_wrap() and text_set_wrap(). In order to
 * enable text wrapping in the first place, use text_set_max_width().
 */
typedef enum text_wrap {
    /** Wrap text on each character */
    TEXT_WRAP_CHARACTER = 0,
    /** Wrap text on each word, denoted by whitespace */
    TEXT_WRAP_WORD,

    TEXT_WRAP_LAST    = TEXT_WRAP_WORD,
    TEXT_WRAP_DEFAULT = TEXT_WRAP_CHARACTER
} text_wrap;

/** @brief Creates a new text object with the provided string format
 *
 * Like text_set_str(), this function uses printf-style formatting.
 *
 * @param s The format string
 * @see text_free()
 * @see saprintf()
 */
text text_new(font f, const char* s, ...);

/** @brief Frees a text object and invalidates its pointer
 *
 * @see _text_free()
 */
#define text_free(t, f) { _text_free(t, f); t = NULL; }

/** @brief Frees a text object
 *
 * Please use text_free() to ensure that the pointer's value is set to NULL
 * when calling.
 *
 * @param f Should the text object's associated font be freed?
 */
void _text_free(text t, bool free_src);

/** @brief Sets the text object's content
 *
 * This function uses printf-style formatting, and will trigger a mesh update.
 *
 * @param s The format string
 * @see saprintf()
 */
void text_set_str(text t, const char* s, ...);

/** @brief Gets the text object's content
 *
 */
char* text_get_str(const text t);

/** @brief Gets the text object's generated mesh
 *
 */
mesh text_get_mesh(const text t);

/** @brief Sets the text object's alignment
 *
 * This affects both the text mesh's position relative to its origin point, and
 * how the text aligns separate lines
 *
 * @param align The text alignment
 */
void text_set_align(text t, alignment_2d align);

/** @brief Gets the text object's alignment
 *
 */
alignment_2d text_get_align(const text t);

/** @brief Sets the text object's wrapping mode
 *
 * In order for the text to wrap, the text must have a non-zero maximum width.
 *
 * @param wrap The wrap mode
 * @see text_set_max_width()
 */
void text_set_wrap(text t, text_wrap wrap);

/** @brief Gets the text object's wrapping mode
 *
 */
text_wrap text_get_wrap(const text t);

/** @brief Gets the maximum width of the text
 *
 */
float text_get_max_width(const text t);

/** @brief Sets the maximum width of the text
 *
 * Setting the width to a non-zero value will cause the text to wrap according
 * to the text's wrapping mode.
 *
 * @param width The maximum width of the text (in pixels/mesh units)
 * @see text_wrap
 */
void text_set_max_width(text t, float width);

/** @brief Gets the actual width and height of text object
 *
 * This value is cached when the text mesh is generated
 */
vec2 text_get_bounds(const text t);

/** @brief Gets the text object's font
 *
 * This font should be used for all rendering purposes when not using
 * text_draw()
 */
font text_get_font(const text t);

/** @brief Sets the text object's font
 *
 * The font is used for both layout and rendering, and will trigger a mesh update
 *
 * @param f The font to use in the text object
 */
void text_set_font(text t, font f);

/** @brief Helper function to render text
 *
 * Renders the text object's generated mesh using the provided shader and
 * transformation matrix
 *
 * @param s The shader program to use. Inputs should match those of the basic_tex shader.
 * @param m The matrix to pass as a transformation
 */
void text_draw(const text t, shader s, mat4 m);

#endif
