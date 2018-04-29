#include "scene.h"


void Object::loadObject(objl::Loader L) {
    Loader = L;
}

void Object::loadObject(HACD::HeapManager * heapManager, size_t triNum) {
    myHACD = HACD::CreateHACD(heapManager);
    
    myHACD->SetPoints(&points[0]);
	myHACD->SetNPoints(points.size());
	myHACD->SetTriangles(&triangles[0]);
	myHACD->SetNTriangles(triangles.size());
	myHACD->SetCompacityWeight(0.0001);
    myHACD->SetVolumeWeight(0.0);
    myHACD->SetConnectDist(30);

    myHACD->SetNClusters(2);                               // minimum number of clusters
    myHACD->SetNVerticesPerCH(100);                        // max of 100 vertices per convex-hull
	myHACD->SetConcavity(0);                               // maximum concavity
	myHACD->SetSmallClusterThreshold(0.25);				   // threshold to detect small clusters
	myHACD->SetNTargetTrianglesDecimatedMesh(triNum);      // # triangles in the decimated mesh
	myHACD->SetCallBack(&CallBack);
    myHACD->SetAddExtraDistPoints(1);   
    myHACD->SetAddFacesPoints(1);
}

void Object::decompose() {
    clock_t start, end;
    double elapsed;
    start = clock();
    {
	    myHACD->Compute();
    }
    end = clock();
    elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    std::cout << "Time " << elapsed << " s"<< endl;
    size_t nClusters = myHACD->GetNClusters();

    //print info

}

void Scene::loadScene(const char* path) {

    string filename = path;
    int found = filename.find_last_of("/");
	if (found != -1)
		folder = filename.substr(0,found) + "/";
	if (folder == "")
        folder = "./";
    file = filename.substr(found+1);

    string format = path;
    if (format.find(".json") != string::npos) {
        cerr << ".json doesn't work yet\n";
        // parseJSON(path);
    }
    else if (format.find(".obj") != string::npos) {
        cerr << ".obj doesn't work yet\n";
        // parseOBJ(path);
    }
    else if (format.find(".off") != string::npos) {
        parseOFF(path);
    }
    else {
        cerr << "Scene file has wrong format\n";
        cerr << "Allow formats: .obj, .off, .json (from threejs.org/)\n";
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
        Object obj(Loader, Loader.LoadedMeshes[0].MeshName);
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

        Object obj(vertices, indices, j_objects["name"]);
        objects.push_back(obj);
    }
}

void Scene::parseOFF(const char* name) {
    Object obj(file);
    string outWRLFileName = folder + file.substr(0, file.find_last_of(".")) + ".wrl";
	string outOFFFileName = folder + file.substr(0, file.find_last_of(".")) + ".off";
    string outOFFFileNameDecimated = folder + file.substr(0, file.find_last_of(".")) + "_decimated.off";
    LoadOFF(name, obj.points, obj.triangles, 0);
	SaveVRML2(outWRLFileName.c_str(), obj.points, obj.triangles);
	SaveOFF(outOFFFileName.c_str(), obj.points, obj.triangles);

    HACD::HeapManager * heapManager = HACD::createHeapManager(65536*(1000));
    obj.loadObject(heapManager, triNum);
    objects.push_back(obj);
    start_decomposition();

	string outFileName = folder + file.substr(0, file.find_last_of(".")) + "_hacd.wrl";
	obj.myHACD->Save(outFileName.c_str(), false);    
    
    const HACD::Vec3<HACD::Real> * const decimatedPoints = obj.myHACD->GetDecimatedPoints();
    const HACD::Vec3<long> * const decimatedTriangles    = obj.myHACD->GetDecimatedTriangles();
    if (decimatedPoints && decimatedTriangles)
    {
        SaveOFF(outOFFFileNameDecimated, obj.myHACD->GetNDecimatedPoints(), 
                                         obj.myHACD->GetNDecimatedTriangles(), decimatedPoints, decimatedTriangles);
    }
    HACD::DestroyHACD(obj.myHACD);
    HACD::releaseHeapManager(heapManager);
}

void Scene::start_decomposition() {
    for (vector<Object>::iterator it = objects.begin() ; it != objects.end(); ++it){
        cout << "Decomposing: " << it->name << endl;
        it->decompose();
    }
}
