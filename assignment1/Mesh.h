#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

typedef unsigned int uint;

// for pointer in struct to point to itself
typedef struct HEEdge HEEdge;

//data structure of indexed face set
typedef struct {
	//3d coordinates
	float x;
	float y;
	float z;
} Vertex;

typedef struct{
	//three vertex ids
	uint a,b,c;
} Face;

// data structure of Halfedge
// http://www.openmesh.org/Documentation/OpenMesh-Doc-Latest/a00016.html

typedef struct {
	//add members here
	HEEdge* edge; 

} HEVertex;

struct HEEdge{
	//add members here

	Face* face;
	HEVertex* vertex;
	HEEdge* opposite;
	HEEdge* next;

};

typedef struct {
	
	HEEdge* Edge;

} HEFace;

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