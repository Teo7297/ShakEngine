#pragma once

// SFML modules
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

// SFML wrappers/utilities
#include <glad/gl.h>
// #include <SFML/OpenGL.hpp>
#include "glCheck.h" //! This is a copy of glCheck.hpp from the SFML.

// STL
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <string>
#include <stack>
#include <memory>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <filesystem>
namespace fs = std::filesystem;


#define DEFAULT_APP_WIDTH 1920U
#define DEFAULT_APP_HEIGHT 1080U