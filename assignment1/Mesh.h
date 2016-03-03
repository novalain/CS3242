#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <assert.h>

// Forward declare these
typedef struct HEEdge HEEdge;
typedef struct HEFace HEFace;

//data structure of indexed face set
struct Vertex {
	//3d coordinates
	float x;
	float y;
	float z;
} ;

struct Face{
	//three vertex ids
	uint a,b,c;
};

struct Edge{
	Vertex v1;
	Vertex v2;
};

// data structure of Halfedge
// http://www.openmesh.org/Documentation/OpenMesh-Doc-Latest/a00016.html

struct HEVertex{
	//add members here
	Vertex vertex;
	HEEdge* edge; 

};


struct HEEdge{
	//add members here

	HEFace* face;
	HEVertex* vertex;
	HEEdge* opposite;
	HEEdge* next;
	int id;

};

struct HEFace {
	
	HEEdge* edge;
	int id;
};

class Mesh{
private:
	std::vector<Vertex> V;
	std::vector<Face> F;

	std::vector<HEVertex> HEV;
	std::vector<HEEdge> HEE;
	std::vector<HEFace> HEF;
public:
	Mesh() {};
	Mesh(const char*);
	//load a Mesh from .mesh file
	void loadMF(const char*);
	//write a Mesh to .mesh file (no header)
	void writeMF(const char*);
	//simplify a mesh
	void simplifyMesh(const char* input, const char* output, int faceCnt);
	//turn indexed face set to halfedge
	void convertMesh();
	//turn halfedge to indexed face set
	void revertMesh();
	//helper methods
	std::vector<HEVertex> neighborVertices(HEVertex v);
	std::vector<HEFace> neighborFaces(HEVertex v);
	std::vector<HEVertex> adjacentVertices(HEFace f);
	//return vertex count
	int Vcnt();
	//return face count
	int Fcnt();


};
#endif