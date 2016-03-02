#include "Mesh.h"

Mesh::Mesh(const char* filename){	
	loadMF(filename);
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
	//writeMF();
}

/* Convert the mesh into the half-edge data structure*/
void Mesh::convertMesh(){

	int count = 0;

	// We need 
	std::map<std::pair<int,int>, HEEdge> edges;
	std::vector<int> added_vertices;

	// Go through all faces
	for(int i = 0; i < F.size(); i++){

		count++;

		// Declare the three half edges, assume that the vertices
		// is declared in a clockwise order
		HEEdge _he1 = {}, _he2 = {}, _he3 = {};
		HEVertex _hevertex1 = {}, _hevertex2 = {}, _hevertex3 = {};
		HEFace _heface = {};

		// HEFace
		_heface.edge = &_he1;
		HEF.push_back(_heface);
		
		// Check if vertices is added alread
		if(std::find(added_vertices.begin(), added_vertices.end(), F[i].a) != added_vertices.end()){

			_hevertex1.vertex = V[F[i].a];
			_hevertex1.edge = &_he1;
			added_vertices.push_back(F[i].a);
			HEV.push_back(_hevertex1);

		}

		if(std::find(added_vertices.begin(), added_vertices.end(), F[i].b) != added_vertices.end()){

			_hevertex2.vertex = V[F[i].b];
			_hevertex2.edge = &_he2;
			added_vertices.push_back(F[i].b);
			HEV.push_back(_hevertex2);

		}

		if(std::find(added_vertices.begin(), added_vertices.end(), F[i].c) != added_vertices.end()){

			_hevertex3.vertex = V[F[i].c];
			_hevertex3.edge = &_he3;
			added_vertices.push_back(F[i].c);
			HEV.push_back(_hevertex3);

		}

		_he1.next = &_he2;
		_he2.next = &_he3;
		_he3.next = &_he1;

		edges[std::make_pair(F[i].a, F[i].b)] = _he1;
		edges[std::make_pair(F[i].b, F[i].c)] = _he2;
		edges[std::make_pair(F[i].c, F[i].a)] = _he3;

		HEE.push_back(_he1);
		HEE.push_back(_he2);
		HEE.push_back(_he3);

		// If opposite edge
		if(edges.find(std::make_pair(F[i].b, F[i].a)) != edges.end()){

			edges[std::make_pair(F[i].a, F[i].b)].opposite = &edges[std::make_pair(F[i].b, F[i].a)];
			edges[std::make_pair(F[i].b, F[i].a)].opposite = &edges[std::make_pair(F[i].a, F[i].b)];

		}

		// If opposite edge
		if(edges.find(std::make_pair(F[i].c, F[i].b)) != edges.end()){

			edges[std::make_pair(F[i].b, F[i].c)].opposite = &edges[std::make_pair(F[i].c, F[i].b)];
			edges[std::make_pair(F[i].c, F[i].b)].opposite = &edges[std::make_pair(F[i].b, F[i].c)];

		}

		// If opposite edge
		if(edges.find(std::make_pair(F[i].a, F[i].c)) != edges.end()){

			edges[std::make_pair(F[i].c, F[i].a)].opposite = &edges[std::make_pair(F[i].a, F[i].c)];
			edges[std::make_pair(F[i].a, F[i].c)].opposite = &edges[std::make_pair(F[i].c, F[i].a)];

		}

	}

	assert(V.size() == HEV.size());
	assert(F.size() == HEF.size());
	assert(HEE.size() == HEF.size()*3);
	V.clear();
	F.clear();

}

void Mesh::revertMesh(){

	std::map<std::pair<int, int>, HEEDge> added_edges;

	for(int i = 0; i < HEF.size(); i++){

		Face f = {};
		Vertex v1 = {}, v2 = {}, v3 = {};

		HEEdge e = HEF[i];
		HEVertex hv1 = e->vertex;
		HEVertex hv2 = e->next->vertex;
		HEVertex hv3 = e->next->next->vertex;

		v1.vertex = hv1.vertex;
		v2.vertex = hv2.vertex;
		v3.vertex = hv3.vertex;

		f.a = 




	}





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