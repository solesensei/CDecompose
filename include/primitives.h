#ifndef H_PRIMITIVES
#define H_PRIMITIVES

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using glm::vec3;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct Box
{
    vec3 boxMin, boxMax;
    vec3 color;
    Box(const vec3 vmin, const vec3 vmax, const vec3 col): boxMin(vmin), boxMax(vmax), color(col){}

};

struct Plane
{
    vec3 lbot, rtop;
    vec3 color;
    Plane(const vec3 vlb, const vec3 vrt, const vec3 col): lbot(vlb), rtop(vrt), color(col){}
};


#endif