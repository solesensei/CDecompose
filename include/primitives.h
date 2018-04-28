#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "nlohmann/json.hpp"


using glm::vec3;
using std::tuple;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::ios;
using nlohmann::json;


struct Point
{
    int x, y, z;
};
 

struct Box
{
    vec3 boxMin, boxMax;
    vec3 color;
    Box(const vec3 vmin, const vec3 vmax, const vec3 col): boxMin(vmin), boxMax(vmax), color(col) {}

};

struct Plane
{
    vec3 lbot, rtop;
    vec3 color;
    Plane(const vec3 vlb, const vec3 vrt, const vec3 col): lbot(vlb), rtop(vrt), color(col) {}
};