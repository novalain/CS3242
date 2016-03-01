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
	//revertMesh();
	//writeMF();
}

/* Convert the mesh into the half-edge data structure*/
void Mesh::convertMesh(){

	/*for(int i = 0; i < V.size(); i++){

		std::cout << "V[i]" << V[i].x << std::endl;

	}*/

	for(int i = 0; i < V.size(); i++){

		std::cout << "V[i]" << V[i].x << std::endl;

	}





}

void Mesh::revertMesh(){
}

std::vector<HEVertex> Mesh::neighborVertices(HEVertex v){
	return std::vector<HEVertex>();
}

std::vector<HEFace> Mesh::neighborFaces(HEVertex v){
	return std::vector<HEVertex>();
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