#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "HACD_Lib/inc/hacdHACD.h"
#include "HACD_Lib/inc/hacdMicroAllocator.h"


inline void CallBack(const char * msg, double progress, double concavity, size_t nVertices) 
{
	std::cout << msg;
}
bool LoadOFF(const std::string & fileName, std::vector< HACD::Vec3<HACD::Real> > & points, std::vector< HACD::Vec3<long> > & triangles, bool invert);
bool SaveOFF(const std::string & fileName, size_t nV, size_t nT, const HACD::Vec3<HACD::Real> * const points, const HACD::Vec3<long> * const triangles);
bool SaveOFF(const std::string & fileName, const std::vector< HACD::Vec3<HACD::Real> > & points, const std::vector< HACD::Vec3<long> > & triangles);
bool SaveVRML2(const std::string & fileName, const std::vector< HACD::Vec3< HACD::Real > > & points, 
               const std::vector< HACD::Vec3<long> > & triangles, const HACD::Vec3<HACD::Real> * colors= 0);
bool SaveVRML2(std::ofstream & fout, const std::vector< HACD::Vec3< HACD::Real > > & points, 
               const std::vector< HACD::Vec3<long> > & triangles,
               const HACD::Material & material, const HACD::Vec3<HACD::Real> * colors = 0);
bool SavePartition(const std::string & fileName, const std::vector< HACD::Vec3<HACD::Real> > & points, 
                   const std::vector< HACD::Vec3<long> > & triangles,
                   const long * partition, const size_t nClusters);


inline bool LoadOFF(const std::string & fileName, std::vector< HACD::Vec3<HACD::Real> > & points, std::vector< HACD::Vec3<long> > & triangles, bool invert) 
{    
	FILE * fid = fopen(fileName.c_str(), "r");
	if (fid) 
    {
		const std::string strOFF("OFF");
		char temp[1024];
		fscanf(fid, "%s", temp);
		if (std::string(temp) != strOFF)
		{
			printf( "Loading error: format not recognized \n");
            fclose(fid);

			return false;            
		}
		else
		{
			int nv = 0;
			int nf = 0;
			int ne = 0;
			fscanf(fid, "%i", &nv);
			fscanf(fid, "%i", &nf);
			fscanf(fid, "%i", &ne);
            points.resize(nv);
			triangles.resize(nf);
            HACD::Vec3<HACD::Real> coord;
			float x = 0;
			float y = 0;
			float z = 0;
			for (long p = 0; p < nv ; p++) 
            {
				fscanf(fid, "%f", &x);
				fscanf(fid, "%f", &y);
				fscanf(fid, "%f", &z);
				points[p].X() = x;
				points[p].Y() = y;
				points[p].Z() = z;
			}        
			int i = 0;
			int j = 0;
			int k = 0;
			int s = 0;
			for (long t = 0; t < nf ; ++t) {
				fscanf(fid, "%i", &s);
				if (s == 3)
				{
					fscanf(fid, "%i", &i);
					fscanf(fid, "%i", &j);
					fscanf(fid, "%i", &k);
					triangles[t].X() = i;
					if (invert)
					{
						triangles[t].Y() = k;
						triangles[t].Z() = j;
					}
					else
					{
						triangles[t].Y() = j;
						triangles[t].Z() = k;
					}
				}
				else			// Fix me: support only triangular meshes
				{
					for(long h = 0; h < s; ++h) fscanf(fid, "%i", &s);
				}
			}
            fclose(fid);
		}
	}
	else 
    {
		printf( "Loading error: file not found \n");
		return false;
    }
	return true;
}

inline bool SaveOFF(const std::string & fileName, const std::vector< HACD::Vec3<HACD::Real> > & points, const std::vector< HACD::Vec3<long> > & triangles)
{
	return SaveOFF(fileName,points.size(), triangles.size(), &points[0], &triangles[0]);
}
inline bool SaveOFF(const std::string & fileName, size_t nV, size_t nT, const HACD::Vec3<HACD::Real> * const points, const HACD::Vec3<long> * const triangles)
{
    std::cout << "Saving " <<  fileName << std::endl;
    std::ofstream fout(fileName.c_str());
    if (fout.is_open()) 
    {           
        fout <<"OFF" << std::endl;	    	
        fout << nV << " " << nT << " " << 0<< std::endl;		
        for(size_t v = 0; v < nV; v++)
        {
            fout << points[v].X() << " " 
                 << points[v].Y() << " " 
                 << points[v].Z() << std::endl;
		}
        for(size_t f = 0; f < nT; f++)
        {
            fout <<"3 " << triangles[f].X() << " " 
                        << triangles[f].Y() << " "                                                  
                        << triangles[f].Z() << std::endl;
        }
        fout.close();
	    return true;
    }
    return false;
}

inline bool SaveVRML2(const std::string & fileName, const std::vector< HACD::Vec3<HACD::Real> > & points, 
               const std::vector< HACD::Vec3<long> > & triangles,
               const HACD::Vec3<HACD::Real> * colors)
{
    std::cout << "Saving " <<  fileName << std::endl;
    std::ofstream fout(fileName.c_str());
    if (fout.is_open()) 
    {
        const HACD::Material material;
        
        if (SaveVRML2(fout, points, triangles, material, colors))
        {
            fout.close();
            return true;
        }
        return false;
    }
    return false;
}

inline bool SaveVRML2(std::ofstream & fout, const std::vector< HACD::Vec3<HACD::Real> > & points, 
               const std::vector< HACD::Vec3<long> > & triangles, 
               const HACD::Material & material, const HACD::Vec3<HACD::Real> * colors)
{
    if (fout.is_open()) 
    {
        size_t nV = points.size();
        size_t nT = triangles.size();            
        fout <<"#VRML V2.0 utf8" << std::endl;	    	
        fout <<"" << std::endl;
        fout <<"# Vertices: " << nV << std::endl;		
        fout <<"# Triangles: " << nT << std::endl;		
        fout <<"" << std::endl;
        fout <<"Group {" << std::endl;
        fout <<"	children [" << std::endl;
        fout <<"		Shape {" << std::endl;
        fout <<"			appearance Appearance {" << std::endl;
        fout <<"				material Material {" << std::endl;
        fout <<"					diffuseColor "      << material.m_diffuseColor.X()      << " " 
                                                        << material.m_diffuseColor.Y()      << " "
                                                        << material.m_diffuseColor.Z()      << std::endl;  
        fout <<"					ambientIntensity "  << material.m_ambientIntensity      << std::endl;
        fout <<"					specularColor "     << material.m_specularColor.X()     << " " 
                                                        << material.m_specularColor.Y()     << " "
                                                        << material.m_specularColor.Z()     << std::endl; 
        fout <<"					emissiveColor "     << material.m_emissiveColor.X()     << " " 
                                                        << material.m_emissiveColor.Y()     << " "
                                                        << material.m_emissiveColor.Z()     << std::endl; 
        fout <<"					shininess "         << material.m_shininess             << std::endl;
        fout <<"					transparency "      << material.m_transparency          << std::endl;
        fout <<"				}" << std::endl;
        fout <<"			}" << std::endl;
        fout <<"			geometry IndexedFaceSet {" << std::endl;
        fout <<"				ccw TRUE" << std::endl;
        fout <<"				solid TRUE" << std::endl;
        fout <<"				convex TRUE" << std::endl;
        if (colors && nT>0)
        {
            fout <<"				colorPerVertex FALSE" << std::endl;
            fout <<"				color Color {" << std::endl;
            fout <<"					color [" << std::endl;
            for(size_t c = 0; c < nT; c++)
            {
                fout <<"						" << colors[c].X() << " " 
                                                  << colors[c].Y() << " " 
                                                  << colors[c].Z() << "," << std::endl;
            }
            fout <<"					]" << std::endl;
            fout <<"				}" << std::endl;
                    }
        if (nV > 0) 
        {
            fout <<"				coord DEF co Coordinate {" << std::endl;
            fout <<"					point [" << std::endl;
            for(size_t v = 0; v < nV; v++)
            {
                fout <<"						" << points[v].X() << " " 
                                                  << points[v].Y() << " " 
                                                  << points[v].Z() << "," << std::endl;
            }
            fout <<"					]" << std::endl;
            fout <<"				}" << std::endl;
        }
        if (nT > 0) 
        {
            fout <<"				coordIndex [ " << std::endl;
            for(size_t f = 0; f < nT; f++)
            {
                fout <<"						" << triangles[f].X() << ", " 
                                                  << triangles[f].Y() << ", "                                                  
                                                  << triangles[f].Z() << ", -1," << std::endl;
            }
            fout <<"				]" << std::endl;
        }
        fout <<"			}" << std::endl;
        fout <<"		}" << std::endl;
        fout <<"	]" << std::endl;
        fout <<"}" << std::endl;	
	    return true;
    }
    return false;
}

inline bool SavePartition(const std::string & fileName, const std::vector< HACD::Vec3<HACD::Real> > & points, 
                                                 const std::vector< HACD::Vec3<long> > & triangles,
                                                 const long * partition, const size_t nClusters)
{
    if (!partition)
    {
        return false;
    }
    
    std::cout << "Saving " <<  fileName << std::endl;
    std::ofstream fout(fileName.c_str());
    if (fout.is_open()) 
    {
        HACD::Material mat;
        std::vector< HACD::Vec3<long> > triCluster;
        std::vector< HACD::Vec3<HACD::Real> > ptsCluster;
        std::vector< long > ptsMap;
        for(size_t c = 0; c < nClusters; c++)
        {
            ptsMap.resize(points.size());
            mat.m_diffuseColor.X() = mat.m_diffuseColor.Y() = mat.m_diffuseColor.Z() = 0.0;
            while (mat.m_diffuseColor.X() == mat.m_diffuseColor.Y() ||
                   mat.m_diffuseColor.Z() == mat.m_diffuseColor.Y() ||
                   mat.m_diffuseColor.Z() == mat.m_diffuseColor.X()  )
            {
                mat.m_diffuseColor.X() = (rand()%100) / 100.0;
                mat.m_diffuseColor.Y() = (rand()%100) / 100.0;
                mat.m_diffuseColor.Z() = (rand()%100) / 100.0;
            }
            long ver[3];
            long vCount = 1;
            for(size_t t = 0; t < triangles.size(); t++)
            {
                if (partition[t] == static_cast<long>(c))
                {
                    ver[0] = triangles[t].X();
                    ver[1] = triangles[t].Y();
                    ver[2] = triangles[t].Z();
                    for(int k = 0; k < 3; k++)
                    {
                        if (ptsMap[ver[k]] == 0)
                        {
                            ptsCluster.push_back(points[ver[k]]);
                            ptsMap[ver[k]] = vCount;
                            ver[k] = vCount-1;
                            vCount++;
                        }
                        else
                        {
                            ver[k] = ptsMap[ver[k]]-1;
                        }
                    }
                    triCluster.push_back(HACD::Vec3<long>(ver[0], ver[1], ver[2]));
                }
            }
            SaveVRML2(fout, ptsCluster, triCluster, mat);
            triCluster.clear();
            ptsCluster.clear();
            ptsMap.clear();
        }

        fout.close();
        return true;
    }
    return false;    
}