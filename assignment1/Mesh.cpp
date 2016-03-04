#include "Mesh.h"

Mesh::Mesh(const char* filename){	
	loadMF(filename);
}

bool operator<(const HEVertex& hev1, const HEVertex& hev2) {
		
	if(hev1.vertex.x < hev2.vertex.x) return true;
	if(hev1.vertex.x > hev2.vertex.x) return false;
	// Otherwise equal
	if(hev1.vertex.y < hev2.vertex.y) return true;
	if(hev1.vertex.y > hev2.vertex.y) return false;

	// Otherwise equal
	if(hev1.vertex.z < hev2.vertex.z) return true;
	if(hev1.vertex.z > hev2.vertex.z) return false;

	return false;

}

void Mesh::loadMF(const char* filename){

	if(V.size()>0) V.clear();
	if(F.size()>0) F.clear();
	std::ifstream infile;
	infile.open(filename, std::ios::in);
	std::string strbuff;
	while(std::getline(infile,strbuff)){
		std::stringstream ss;
		ss<<strbuff;
		char type;
		ss>>type;
		if(type=='v'){
			Vertex v;
			ss>>v.x>>v.y>>v.z;
			V.push_back(v);
		}
		else if(type=='f'){
			Face f;
			ss>>f.a>>f.b>>f.c;
			F.push_back(f);
		}
	}
	infile.close();
}

void Mesh::writeMF(const char* filename){
	std::ofstream outfile;
	outfile.open(filename, std::ios::out);
	std::string strbuff;
	for(uint i=0;i<V.size();i++){
		outfile<<"v "<<V[i].x<<" "<<V[i].y<<" "<<V[i].z<<std::endl;
	}
	for(uint i=0;i<F.size();i++){
		outfile<<"f "<<F[i].a<<" "<<F[i].b<<" "<<F[i].c<<std::endl;
	}
	outfile.close();
}

// faceCnt = max number of vertices
void Mesh::simplifyMesh(const char* input, const char* output, int faceCnt){
	// you may assume inputs are always valid
	loadMF(input);
	convertMesh();
	std::cout<<"Original face count: "<<HEF.size()<<std::endl;
	// do mesh simplification here
	revertMesh();
	writeMF(output);
}

/* Convert the mesh into the half-edge data structure*/
void Mesh::convertMesh(){

	std::map<std::pair<int,int>, HEEdge*> edges;

	// Add vertices
	for(int i = 0; i < V.size(); i++){
		HEVertex* _hevertex = new HEVertex();
		_hevertex->vertex = V[i];
		HEV.push_back(*_hevertex);
	}

	// Go through all faces
	for(int i = 0; i < F.size(); i++){

		// Declare the variables,
		// Assume that the vertices are declared in a clockwise order
		HEFace* _heface = new HEFace();

		HEEdge* _halfedge1 = new HEEdge();
		HEEdge* _halfedge2 = new HEEdge();
		HEEdge* _halfedge3 = new HEEdge();

		HEVertex* _hevertex1 = &HEV[F[i].a-1];
		HEVertex* _hevertex2 = &HEV[F[i].b-1];
		HEVertex* _hevertex3 = &HEV[F[i].c-1];

		// HEFace
		_heface->edge = _halfedge1; 
		_heface->id = i;
		HEF.push_back(*_heface);

		// Update pointers from halfedge to vertex 
		_halfedge3->vertex = _hevertex1;
		_hevertex1->edge = _halfedge1;

		_hevertex2->edge = _halfedge2;
		_halfedge2->vertex = _hevertex2;

		_hevertex3->edge = _halfedge3;
		_halfedge1->vertex = _hevertex3;

		// Next
		_halfedge1->next = _halfedge2;
		_halfedge2->next = _halfedge3;
		_halfedge3->next = _halfedge1;

		// Face
		_halfedge1->face = _heface;
		_halfedge2->face = _heface;
		_halfedge3->face = _heface;

		edges[std::make_pair(F[i].a, F[i].b)] = _halfedge1;
		edges[std::make_pair(F[i].b, F[i].c)] = _halfedge2;
		edges[std::make_pair(F[i].c, F[i].a)] = _halfedge3;

		HEE.push_back(*_halfedge1);
		HEE.push_back(*_halfedge2);
		HEE.push_back(*_halfedge3);

		// If opposite edge has been added
		if(edges.find(std::make_pair(F[i].b, F[i].a)) != edges.end()){

			edges[std::make_pair(F[i].a, F[i].b)]->opposite = edges[std::make_pair(F[i].b, F[i].a)];
			edges[std::make_pair(F[i].b, F[i].a)]->opposite = edges[std::make_pair(F[i].a, F[i].b)];

		}

		if(edges.find(std::make_pair(F[i].c, F[i].b)) != edges.end()){

			edges[std::make_pair(F[i].b, F[i].c)]->opposite = edges[std::make_pair(F[i].c, F[i].b)];
			edges[std::make_pair(F[i].c, F[i].b)]->opposite = edges[std::make_pair(F[i].b, F[i].c)];

		}

		if(edges.find(std::make_pair(F[i].a, F[i].c)) != edges.end()){

			edges[std::make_pair(F[i].c, F[i].a)]->opposite = edges[std::make_pair(F[i].a, F[i].c)];
			edges[std::make_pair(F[i].a, F[i].c)]->opposite = edges[std::make_pair(F[i].c, F[i].a)];

		}

	}

	// Everything ok?
	assert(V.size() == HEV.size());
	assert(F.size() == HEF.size());
	assert(HEE.size() == HEF.size()*3); // Three Halfedge per face

	V.clear();
	F.clear();

}

void Mesh::revertMesh(){

	std::map<HEVertex, int> indices;

	for(int i = 0; i < HEV.size(); i++){

		Vertex v = {};
		v = HEV[i].vertex;
		V.push_back(v);
		indices[HEV[i]] = i+1;
	}

	assert(V.size() == HEV.size());	

	for(int i = 0; i < HEF.size(); i++){

		Face f = {};

		HEFace _heface = HEF[i];
		HEEdge* _halfedge1 = _heface.edge;

		HEVertex* _hevertex3 = _halfedge1->vertex;
		HEVertex* _hevertex2 = _halfedge1->next->vertex;
		HEVertex* _hevertex1 = _halfedge1->next->next->vertex; 
		
		f.a = indices[*_hevertex1];
		f.b = indices[*_hevertex2];
		f.c = indices[*_hevertex3];

		// No elements in face can be equal
		assert(f.a != f.b && f.b != f.c && f.c != f.a);

		F.push_back(f);

	}

	assert(HEF.size() == F.size());


}

std::vector<HEVertex> Mesh::neighborVertices(HEVertex v){
	return std::vector<HEVertex>();
}

std::vector<HEFace> Mesh::neighborFaces(HEVertex v){
	return std::vector<HEFace>();
}

std::vector<HEVertex> Mesh::adjacentVertices(HEFace f){
	return std::vector<HEVertex>();
}

int Mesh::Vcnt(){
	return HEV.size();
}

int Mesh::Fcnt(){
	return HEF.size();
}