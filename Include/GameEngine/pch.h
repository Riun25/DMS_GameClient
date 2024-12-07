#pragma once
#define NOMINMAX
#include <windows.h>
#include <memory>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdint>

#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include <variant>
#include <source_location>
#include <tuple>
#include <numbers>
#include <limits>
#include <chrono>

#pragma warning(push)
#pragma warning(disable : 26819)
#pragma warning(disable : 26495)
#pragma warning(disable : 5260)
#include <nlohmann/json.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4819)
#pragma warning(disable : 26827)
#include <entt/entt.hpp>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 5260)
#include <DirectXTK/SimpleMath.h>
#pragma warning(pop)



#include "Logger.h"

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::SimpleMath;
using json = nlohmann::json;
