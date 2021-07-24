#pragma once

// Logging
#define BLAZAR_ENABLE_ASSERTS
#define BLAZAR_IMGUI_ENABLED

// Assets
#define BLAZAR_CFG_RESOURCE_LAZYLOAD

// Development
#define BLAZAR_CFG_DEV_ENABLE
#define BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

// Configuration rules

#ifdef BLAZAR_RELEASE
#undef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#endif

#ifdef BLAZAR_DIST
#undef BLAZAR_ENABLE_ASSERTS
#undef BLAZAR_CFG_DEV_ENABLE
#endif