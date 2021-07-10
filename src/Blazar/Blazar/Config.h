#pragma once

// Logging
#define BLAZAR_CONFIG_LOG_RESOURCE_USAGE
#define BLAZAR_ENABLE_ASSERTS
#define BLAZAR_IMGUI_ENABLED

// Configuration rules
#ifdef BLAZAR_DIST
#undef BLAZAR_ENABLE_ASSERTS
#endif