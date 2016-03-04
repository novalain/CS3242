#include "Mesh.h"

Mesh::Mesh(const char* filename){	
	loadMF(filename);
}

bool operator<(const HEVertex& hev1, const HEVertex& hev2) {
		
	if(hev1.x < hev2.x) return true;
	if(hev1.x > hev2.x) return false;
	// Otherwise equal
	if(hev1.y < hev2.y) return true;
	if(hev1.y > hev2.y) return false;

	// Otherwise equal
	if(hev1.z < hev2.z) return true;
	if(hev1.z > hev2.z) return false;

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
	std::map<int, HEVertex*> added_vertices;

	// Go through all faces
	for(int i = 0; i < F.size(); i++){

		// Declare the three half edges, assume that the vertices
		// is declared in a clockwise order
		HEFace* _heface = new HEFace();

		HEVertex* _hevertex1 = new HEVertex();
		HEVertex* _hevertex2 = new HEVertex();
		HEVertex* _hevertex3 = new HEVertex();

		HEEdge* _halfedge1 = new HEEdge();
		HEEdge* _halfedge2 = new HEEdge();
		HEEdge* _halfedge3 = new HEEdge();

		// HEFace
		_heface->edge = _halfedge1; 
		_heface->id = i;
		HEF.push_back(*_heface);

		// Get HEVertex and HEEdge
		_hevertex1->x = V[F[i].a].x;
		_hevertex1->y = V[F[i].a].y;
		_hevertex1->z = V[F[i].a].z;

		// Get HEVertex and HEEdge
		_hevertex2->x = V[F[i].b].x;
		_hevertex2->y = V[F[i].b].y;
		_hevertex2->z = V[F[i].b].z;

		// Get HEVertex and HEEdge
		_hevertex3->x = V[F[i].c].x;
		_hevertex3->y = V[F[i].c].y;
		_hevertex3->z = V[F[i].c].z;

		_halfedge3->vertex = _hevertex1;
		_hevertex1->edge = _halfedge1;

		_hevertex2->edge = _halfedge2;
		_halfedge2->vertex = _hevertex2;

		_hevertex3->edge = _halfedge3;
		_halfedge1->vertex = _hevertex3;

		// Check if vertices is added alread
		if(added_vertices.find(F[i].a) == added_vertices.end()){

			added_vertices[F[i].a] = _hevertex1;
			HEV.push_back(*_hevertex1);

		} 
		/*else {

					
			//_hevertex1 = added_vertices[F[i].a];
			
			//halfedge3->vertex = added_vertices[F[i].a];
			
			if(F[i].a == 2306){
				std::cout << "GETTING VERTEX NUMBER 2306 in a" << std::endl;
				std::cout << " Vertex num " << F[i].a << " already existed and halfedge3 pointing there: " << _halfedge3->vertex->x << std::endl;
			}
		}
*/

		// Check if vertices is added 
		if(added_vertices.find(F[i].b) == added_vertices.end()){

			
			added_vertices[F[i].b] = _hevertex2;
			HEV.push_back(*_hevertex2);

		} /*else {


			//_halfedge2->vertex = added_vertices[F[i].b];
		
		}*/

		// Check if vertices is added alread
		if(added_vertices.find(F[i].c) == added_vertices.end()){

			added_vertices[F[i].c] = _hevertex3;
			HEV.push_back(*_hevertex3);

		} /*else {
			
			//_halfedge1->vertex = added_vertices[F[i].c];
		}*/

		/*if(i == 4785){

			std::cout << "F[i].a " << F[i].a << std::endl;
			std::cout << "F[i].b " << F[i].b << std::endl;
			std::cout << "F[i].c " << F[i].c << std::endl;

			std::cout << "V[F[i].a].x " << V[F[i].a].x << std::endl;
			std::cout << "V[F[i].b].x " << V[F[i].b].x << std::endl;
			std::cout << "V[F[i].c].x " << V[F[i].c].x << std::endl;


			std::cout << "V[F[i].a].x " << V[F[i].a].y << std::endl;
			std::cout << "V[F[i].b].x " << V[F[i].b].y << std::endl;
			std::cout << "V[F[i].c].x " << V[F[i].c].y << std::endl;


			std::cout << "V[F[i].a].x " << V[F[i].a].z << std::endl;
			std::cout << "V[F[i].b].x " << V[F[i].b].z << std::endl;
			std::cout << "V[F[i].c].x " << V[F[i].c].z << std::endl;


			std::cout << "hevertex1x : " << _hevertex1->x << std::endl;
			std::cout << "hevertex1y : " << _hevertex1->y << std::endl;
			std::cout << "hevertex1z : " << _hevertex1->z << std::endl;

			std::cout << "hevertex2x : " << _hevertex2->x << std::endl;
			std::cout << "hevertex2y: "  << _hevertex2->y << std::endl;
			std::cout << "hevertex2z : " << _hevertex2->z << std::endl;

			std::cout << "hevertex3x : " << _hevertex3->x << std::endl;
			std::cout << "hevertex3y : " << _hevertex3->y << std::endl;
			std::cout << "hevertex3z : " << _hevertex3->z << std::endl;

		}*/

		// Next
		_halfedge1->next = _halfedge2;
		_halfedge2->next = _halfedge3;
		_halfedge3->next = _halfedge1;

		// Face
		_halfedge1->face = _heface;
		_halfedge2->face = _heface;
		_halfedge3->face = _heface;


	/*	std::cout << " << face has : " << F[i].a << " , " << F[i].b << " , " << F[i].c << std::endl
				  << " and vertices x pos " << V[F[i].a].x << " , " << V[F[i].b].x << " , " << V[F[i].c].x << std::endl; 

		std::cout << " Pushing back _heface with edges: " << std::endl
				  << " halfedge 1 pointing to vertex x pos  " << _halfedge1.vertex->x << std::endl
				  << " halfedge 2 pointing to vertex x pos " << _halfedge2.vertex->x << std::endl
				  << " halfedge 3 pointing to vertex x pos " << _halfedge3.vertex->x << std::endl;

	 	std::cout << " ===================================  " << std::endl;*/
		

		//std::cout << "face? " << _heface.edge->next->vertex->vertex.x << std::endl;

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

	std::cout << "V size " << V.size() << std::endl;
	

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

	std::cout << "HEV size " << HEV.size() << std::endl;

	for(int i = 0; i < HEV.size(); i++){

		//std::cout << "hev vertex x : " << HEV[i].vertex.x << std::endl;
		Vertex v = {};
		v.x = HEV[i].x;
		v.y = HEV[i].y;
		v.z = HEV[i].z;
		V.push_back(v);
		indices[HEV[i]] = i;
	}

	std::cout << " map size " << indices.size() << std::endl;

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

		if(f.a == f.b||f.b == f.c||f.c == f.a){


			std::cout << "f.a " << f.a << std::endl;
			std::cout << "f.b " << f.b << std::endl;
			std::cout << "f.c " << f.c << std::endl;
			std::cout << "at face with id " << HEF[i].id << std::endl;
			std::cout << "================================================" << std::endl;
			
		} 		

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