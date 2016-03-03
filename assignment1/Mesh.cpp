#include "Mesh.h"

Mesh::Mesh(const char* filename){	
	loadMF(filename);
}


bool operator<(const HEVertex hev1, const HEVertex hev2) {
	return hev1.vertex.x < hev2.vertex.x;
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

	//int count = 0;

	std::map<std::pair<int,int>, HEEdge> edges;
	std::map<int, HEVertex> added_vertices;

	// Go through all faces
	for(int i = 0; i < F.size(); i++){

		//count++;

		// Declare the three half edges, assume that the vertices
		// is declared in a clockwise order
		HEEdge _halfedge1 = {}, _halfedge2 = {}, _halfedge3 = {};
		HEVertex _hevertex1 = {}, _hevertex2 = {}, _hevertex3 = {};
		HEFace _heface = {};

		// Check if vertices is added alread
		if(added_vertices.find(F[i].a) == added_vertices.end()){

			_hevertex1.vertex = V[F[i].a];
			_hevertex1.edge = &_halfedge1;
			_halfedge3.vertex = &_hevertex1;
			added_vertices[F[i].a] = _hevertex1;
			HEV.push_back(_hevertex1);

		} else {

			_halfedge3.vertex = &added_vertices[F[i].a];
			//std::cout << " Vertex num " << F[i].a << " already existed and halfedge3 pointing there: " << _halfedge3.vertex->vertex.x << std::endl;

		}


		// Check if vertices is added alread
		if(added_vertices.find(F[i].b) == added_vertices.end()){

			_hevertex2.vertex = V[F[i].b];
			_hevertex2.edge = &_halfedge2;
			_halfedge2.vertex = &_hevertex2;
			added_vertices[F[i].b] = _hevertex2;
			HEV.push_back(_hevertex2);

		} else {

			
			_halfedge2.vertex = &added_vertices[F[i].b];
		//	std::cout << " Vertex num " << F[i].b << " already existed and halfedge2 pointing there: " << _halfedge2.vertex->vertex.x << std::endl;

		}

		// Check if vertices is added alread
		if(added_vertices.find(F[i].c) == added_vertices.end()){

			_hevertex3.vertex = V[F[i].c];
			_hevertex3.edge = &_halfedge3;
			_halfedge1.vertex = &_hevertex3;
			added_vertices[F[i].c] = _hevertex3;
			HEV.push_back(_hevertex3);

		} else {
			_halfedge1.vertex = &added_vertices[F[i].c];
		//	std::cout << " Vertex num " << F[i].c << " already existed and halfedge1 pointing there: " << _halfedge1.vertex->vertex.x << std::endl;

		}

		_halfedge1.next = &_halfedge2;
		_halfedge2.next = &_halfedge3;
		_halfedge3.next = &_halfedge1;

		_halfedge1.face = &_heface;
		_halfedge2.face = &_heface;
		_halfedge3.face = &_heface;


		std::cout << " << face has : " << F[i].a << " , " << F[i].b << " , " << F[i].c << std::endl
				  << " and vertices x pos " << V[F[i].a].x << " , " << V[F[i].b].x << " , " << V[F[i].c].x << std::endl; 

		std::cout << " Pushing back _heface with edges: " << std::endl
				  << " halfedge 1 pointing to vertex x pos  " << _halfedge1.vertex->vertex.x << std::endl
				  << " halfedge 2 pointing to vertex x pos " << _halfedge2.vertex->vertex.x << std::endl
				  << " halfedge 3 pointing to vertex x pos " << _halfedge3.vertex->vertex.x << std::endl;

	 	std::cout << " ===================================  " << std::endl;
		

		std::cout << "YAo 1 " <<  _halfedge1.next->vertex->vertex.x << std::endl;
		std::cout << "YAo 2 " <<  _halfedge2.next->vertex->vertex.x << std::endl;
		std::cout << "YAo 3 " <<  _halfedge3.next->vertex->vertex.x << std::endl;

		//std::cout << "face? " << _heface.edge->next->vertex->vertex.x << std::endl;

		edges[std::make_pair(F[i].a, F[i].b)] = _halfedge1;
		edges[std::make_pair(F[i].b, F[i].c)] = _halfedge2;
		edges[std::make_pair(F[i].c, F[i].a)] = _halfedge3;

		HEE.push_back(_halfedge1);
		HEE.push_back(_halfedge2);
		HEE.push_back(_halfedge3);

		_halfedge1.id = 1;
		_halfedge2.id = 2;
		_halfedge3.id = 3;

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


		// HEFace
		_heface.edge = &_halfedge1;
		_heface.id = 1;
		std::cout << "edge lah" << _halfedge1.vertex->vertex.x << std::endl;;
		
		HEF.push_back(_heface);

		

	}

	std::cout << "V size " << V.size() << std::endl;
	std::cout << "HEV size " << HEV.size() << std::endl;

	//assert(V.size() == HEV.size());
	assert(F.size() == HEF.size());
	assert(HEE.size() == HEF.size()*3);
	V.clear();
	F.clear();

}

void Mesh::revertMesh(){

	//std::map<std::pair<int, int>, HEEdge> added_edges;
	//std::map<Vertex, Vertex> added_vertices;
	std::map<HEVertex, int> indices;

	for(int i = 0; i < HEV.size(); i++){

		//std::cout << "hev vertex x : " << HEV[i].vertex.x << std::endl;
		V.push_back(HEV[i].vertex);
		indices[HEV[i]] = i;
	}

	//assert(V.size() == HEV.size());

	std::cout << "HEF SIZE " << HEF.size() << std::endl;
	std::cout << "first id debug " << HEF[0].edge->id<< std::endl;

	for(int i = 0; i < HEF.size(); i++){

		Face f = {};

		HEFace _heface = HEF[i];
		HEEdge* _halfedge1 = _heface.edge;
		//std::cout << " yeha " << _halfedge1->vertex->vertex.x << std::endl;
		//HEVertex* _hevertex2 = _halfedge1->vertex;
		//HEVertex* _hevertex3 = _halfedge1->next->vertex;

		//std::cout << "vertex x coords " << _halfedge1->vertex->vertex.x << std::endl;
//		std::cout << "vertex x next coords " << _hevertex3->vertex.x << std::endl;

	
//		HEVertex hv1 = *e->next->next->vertex;

//		f.a = indices[hv1];
//		f.b = indices[hv2];
//		f.c = indices[hv3];

		//std::cout << "f.a " << f.a << std::endl;
		//std::cout << "f.b " << f.b << std::endl;
		//std::cout << "f.c " << f.c << std::endl;

//		F.push_back(f);

	}

	//assert(HEF.size() == F.size());


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