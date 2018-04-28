#include "scene.h"


void Object::loadObject(objl::Loader L) {
    Loader = L;
}

bool Object::isConvex(){
    return false;
}
void Object::decompose() {

}


void Scene::loadScene(const char* path) {
    string format = path;
    if (format.find(".json") != string::npos) {
        parseJSON(path);
    }
    else if (format.find(".obj") != string::npos) {
        parseOBJ(path);
    }
    else {
        cerr << "Scene file has wrong format\n";
        cerr << "Allow formats: .obj and .json (from threejs.org/)\n";
    }

}

void Scene::parseOBJ(const char* name) {
    objl::Loader Loader;
    bool loadout = Loader.LoadFile(name);

    if (loadout) {
        cout << "\nLoading meshes:\n";
        for (uint i = 0; i < Loader.LoadedMeshes.size(); i++) {			
            objl::Mesh curMesh = Loader.LoadedMeshes[i];
			cout << curMesh.MeshName << endl;
        }
        Object obj(Loader);
        objects.push_back(obj);
    }
    else {
        cerr << "Loader Error :: Couldn't load the file: " << name << endl;
    }
}

void Scene::parseJSON(const char* name) {
    ifstream file;
    json j;

    file.open(name, ios::in);
    if (file.is_open()) {
        file >> j;
        file.close();
    }
    else {
        cerr << "Unable to open file: " << name << endl;
        exit(1);
    }

    cout << "\nLoading objects:\n";
    
    json j_objects = j["object"]["children"];
    json j_geometries = j["geometries"];
    for (int i = 0; i < j_geometries.size(); ++i) {
        string s = j_geometries[i]["type"];
        cout << s.substr(0,s.size()-14);
       
        // json j_matrix = j_objects[i]["matrix"];
        vector<float> vertices;
        vector<int> indices;
        float w = !j_geometries[i]["width"].is_null() ? float(j_geometries[i]["width"]) : 0;
        float h = !j_geometries[i]["height"].is_null() ?float(j_geometries[i]["height"]) : 0;
        float d = !j_geometries[i]["depth"].is_null() ? float(j_geometries[i]["depth"]) : 0;
        cout << " : " << w << " x " << h << " x " << d << endl;
        if (w) {
            vertices.push_back(w-w/2);
            vertices.push_back(w+w/2);
        }
        if (h) {
            vertices.push_back(h-h/2);
            vertices.push_back(h+h/2);
        }
        if (d) {
            vertices.push_back(d-d/2);
            vertices.push_back(d+d/2);
        }

        Object obj(vertices, indices);
        objects.push_back(obj);
    }
}
