#pragma once
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl/client.h>

#include <DirectXMath.h>

//STL Dependencies
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <optional>
#include <algorithm>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstdint>

#include <vector>
#include <array>
#include <map>
#include <stack>
#include <queue>
#include <unordered_map>
//
//#include "Renderer/Model.h"

#pragma comment(lib,"d3dcompiler.lib")
//#pragma comment(lib,"user32.lib") 

#define NOCTIS_USE_IMGUI 0
