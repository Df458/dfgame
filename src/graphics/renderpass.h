#ifndef DF_GRAPHICS_RENDERPASS
#define DF_GRAPHICS_RENDERPASS

#include "renderpass.hd"

#include "graphics/framebuffer.h"
#include "graphics/shader.hd"

renderpass renderpass_new(uint16 w, uint16 h, framebuffer primary);
#define renderpass_free(p) { _renderpass_free(p); p = NULL; }
void _renderpass_free(renderpass pass);

void renderpass_start(renderpass pass);
void renderpass_next(renderpass pass, shader s);
void renderpass_present(renderpass pass, shader s);

#endif
