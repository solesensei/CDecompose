#pragma once

#include "primitives.h"
#include "OBJ_Loader.h"

class Object
{
    vector<Object> parts;
    objl::Loader Loader;
    vec3 color;
    
    void loadObject(objl::Loader L);
    bool isConvex();
    void decompose();

public:
    vector<float> vertices;
    vector<int> indices; 
    Object(const vector<float> v, const vector<int> i):
        vertices(v), indices(i) 
    {}
    Object(objl::Loader L)
    {
        loadObject(L);
    }

};

class Scene
{
    
    void loadScene(const char* path);
    void parseJSON(const char* name); // JSON exported from threejs.org/
    void parseOBJ(const char* name); // parse .obj file with .mtl if exist
    void saveScene();

public:
    vector<Object> objects;
    Scene(const char* filename) {
        loadScene(filename);
    }

};

