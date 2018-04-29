#pragma once

#include "primitives.h"
#include "OBJ_Loader.h"
#include "OFF_Loader.h"

class Object
{
    vector<Object> parts;
    objl::Loader Loader;
    vec3 color;
    
    void loadObject(objl::Loader L);
    bool isConvex();

public:
    vector<float> vertices;
    vector<int> indices;
    string name;
    vector< HACD::Vec3<HACD::Real> > points;
    vector< HACD::Vec3<long> > triangles;
	HACD::HACD * myHACD;
    
    void loadObject(HACD::HeapManager * heapManager, size_t triNum);    
    void decompose();

    Object(const vector<float> v, const vector<int> i, string n):
        vertices(v), indices(i), name(n)
    {}
    Object(objl::Loader L, string n)
    {
        name = n;
        loadObject(L);
    }

    Object(string n): name(n)
    {}

};

class Scene
{
    string folder, file;
    void loadScene(const char* path);
    void parseJSON(const char* name); // JSON exported from threejs.org/
    void parseOBJ(const char* name); // parse .obj file with .mtl if exist
    void parseOFF(const char* name); // parse .off file
    void saveScene();

public:
    vector<Object> objects;
    int triNum;
    void start_decomposition();
    
    Scene(const char* filename, int tn = 200) {
        triNum = tn;
        loadScene(filename);
    }

};

