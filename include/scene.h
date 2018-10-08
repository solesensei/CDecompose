#pragma once

#include "primitives.h"
#include "OBJ_Loader.h"
#include "OFF_Loader.h"

class Object
{
    vector<Object> parts;
    objl::Loader Loader;
    vec3 color;
    
    bool isConvex();

public:
    string name;
    vector< HACD::Vec3<HACD::Real> > points;
    vector< HACD::Vec3<long> > triangles;
	HACD::HACD * myHACD;
    HACD::HeapManager * heapManager;

    void loadObject(objl::Loader L);
    void loadObject(int triNum);    
    void decompose();

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
    void saveScene(Object obj);

public:
    vector<Object> objects;
    int triNum;
    void start_decomposition();
    
    Scene(const char* filename, int tn = 200) {
        triNum = tn;
        loadScene(filename);
    }

};

