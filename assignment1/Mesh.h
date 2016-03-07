#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
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

	bool operator==(Vertex v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vertex& v) {

		return stream << "Vertex \n" 
			   << " x : " << v.x << "\n" 
			   << " y : " << v.y << "\n"
			   << " z : " << v.z << "\n";

	}

} ;

struct Face{
	//three vertex ids
	uint a,b,c;
	int id;
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
	HEEdge* edge; // One outgoing half-edge

	bool operator<(const HEVertex& hev) const {
		
		if(vertex.x < hev.vertex.x) return true;
		if(vertex.x > hev.vertex.x) return false;
		// Otherwise equal
		if(vertex.y < hev.vertex.y) return true;
		if(vertex.y > hev.vertex.y) return false;

		// Otherwise equal
		if(vertex.z < hev.vertex.z) return true;
		if(vertex.z > hev.vertex.z) return false;

		return false;

	}

	bool operator==(const HEVertex& other) const {
		
		return vertex.x == other.vertex.x && vertex.y == other.vertex.y
			   && vertex.z == other.vertex.z;
		
	}

	bool operator!=(const HEVertex& other) const {
		
		return !(vertex.x == other.vertex.x && vertex.y == other.vertex.y
			   && vertex.z == other.vertex.z);
		
	}

	friend std::ostream& operator<<(std::ostream& stream, const HEVertex& v){

		return stream << "\n" << v.vertex;

	}

};


struct HEEdge{
	//add members here

	HEFace* face;
	HEVertex* hevertex;
	HEEdge* opposite;
	HEEdge* next;
	int id;
	bool exists;

	bool operator==(HEEdge e) const {
		return id == e.id;
	}

	bool operator!=(HEEdge e) const {
		return !(id == e.id);
	}

	bool operator<(const HEEdge& other) const {
		
		if(hevertex->vertex.x < other.hevertex->vertex.x) return true;
		if(hevertex->vertex.x > other.hevertex->vertex.x) return false;
		// Otherwise equal
		if(hevertex->vertex.y < other.hevertex->vertex.y) return true;
		if(hevertex->vertex.y > other.hevertex->vertex.y) return false;

		// Otherwise equal
		if(hevertex->vertex.z < other.hevertex->vertex.z) return true;
		if(hevertex->vertex.z > other.hevertex->vertex.z) return false;

		return false;
		
	}

};

struct HEFace {
	
	HEEdge* edge;
	int id;

	bool operator<(const HEFace& other) const {
		
		if(edge->hevertex->vertex.x < other.edge->hevertex->vertex.x) return true;
		if(edge->hevertex->vertex.x > other.edge->hevertex->vertex.x) return false;
		// Otherwise equal
		if(edge->hevertex->vertex.y < other.edge->hevertex->vertex.y) return true;
		if(edge->hevertex->vertex.y > other.edge->hevertex->vertex.y) return false;

		// Otherwise equal
		if(edge->hevertex->vertex.z < other.edge->hevertex->vertex.z) return true;
		if(edge->hevertex->vertex.z > other.edge->hevertex->vertex.z) return false;

		return false;
		
	}

};

class Mesh{
private:
	std::vector<Vertex> V;
	std::vector<Face> F;
	/*
	std::vector<HEEdge*> HEE;
	std::vector<HEFace*> HEF;

	std::map<HEVertex*, int> HEVToIndex;
	std::map<HEEdge*, int> HEEToIndex;
	std::map<HEFace*, int> HEFToIndex;*/
	std::set<HEVertex*> HEV;
	std::set<HEEdge*> HEE;
	std::set<HEFace*> HEF;

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
	//collapse random
	void collapseRandomEdges(const int faceCnt) ;
	void collapseVertices(HEVertex* v1, HEVertex* v2);
	void debugAdjacentEdges();
	bool canEdgeCollapse(HEEdge* edge);
	int countCommonVertices(HEVertex* v1, HEVertex* v2);
	//helper methods
	std::vector<HEVertex*> getNeighbourVertices(HEVertex* v);
	std::vector<HEFace*> neighborFaces(HEVertex* v);
	std::vector<HEVertex*> adjacentVertices(HEFace* f);
	//return vertex count
	int Vcnt();
	//return face count
	int Fcnt();


};
#endif