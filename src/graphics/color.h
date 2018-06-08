#include "math/vector.h"

static const vec4 color_black   = { .r = 0.0f,  .g = 0.0f,  .b = 0.0f,  .a = 1.0f };
static const vec4 color_white   = { .r = 1.0f,  .g = 1.0f,  .b = 1.0f,  .a = 1.0f };
static const vec4 color_clear   = { .r = 0.0f,  .g = 0.0f,  .b = 0.0f,  .a = 0.0f };

static const vec4 color_red     = { .r = 1.0f,  .g = 0.0f,  .b = 0.0f,  .a = 1.0f };
static const vec4 color_green   = { .r = 0.0f,  .g = 1.0f,  .b = 0.0f,  .a = 1.0f };
static const vec4 color_blue    = { .r = 0.0f,  .g = 0.0f,  .b = 1.0f,  .a = 1.0f };

static const vec4 color_cyan    = { .r = 0.0f,  .g = 1.0f,  .b = 1.0f,  .a = 1.0f };
static const vec4 color_magenta = { .r = 1.0f,  .g = 0.0f,  .b = 1.0f,  .a = 1.0f };
static const vec4 color_yellow  = { .r = 1.0f,  .g = 1.0f,  .b = 0.0f,  .a = 1.0f };

static const vec4 color_dgray   = { .r = 0.25f, .g = 0.25f, .b = 0.25f, .a = 1.0f };
static const vec4 color_gray    = { .r = 0.5f,  .g = 0.5f,  .b = 0.5f,  .a = 1.0f };
static const vec4 color_lgray   = { .r = 0.75f, .g = 0.75f, .b = 0.75f, .a = 1.0f };
