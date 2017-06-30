#ifndef DF_GRAPHICS_RENDERPASS
#define DF_GRAPHICS_RENDERPASS

#include "renderpass.hd"

#include "shader.hd"

renderpass renderpass_new(uint16 w, uint16 h);

void renderpass_start(renderpass pass);
void renderpass_next(renderpass pass, shader s);
void renderpass_present(renderpass pass, shader s);

#endif
