#ifndef _BLAZAR_PCH_H
#define _BLAZAR_PCH_H

// For MSBUILD, disabling warnings on not using their extension methods
#define _CRT_SECURE_NO_WARNINGS

// Standard library
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Blazar specific
#include <Blazar/Core.h>
#include <Blazar/Log.h>

// Globally available libraries
#include <Tracy.hpp>

#endif
