#include "Mesh.h"
#include <iostream>

int main(int argc, char* argv[]){
	if(argc!=4) std::cout<<"Usage: ./exe input output faceCnt\n";
	else{
		Mesh mesh;
		mesh.simplifyMesh(argv[1],argv[2],atoi(argv[3]));
	}
	return 0;
}