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

std::vector<HEVertex*> Mesh::getNeighbourVertices(HEVertex* v){

	std::vector<HEVertex*> tmp;

	//HEEdge* first = v->edge;
	//HEEdge* curr = v->edge->opposite->next;
	HEEdge* curr = v->edge;
	HEEdge* first = v->edge;

	do{
		tmp.push_back(curr->hevertex);
		curr = curr->opposite->next;
	} while(*curr != *first);

	return tmp;
}

int Mesh::countCommonVertices(HEVertex* v1, HEVertex* v2){

	int count = 0;
	std::vector<HEVertex*> v1_neighbours = getNeighbourVertices(v1);
	std::vector<HEVertex*> v2_neighbours = getNeighbourVertices(v2);

	for(int i = 0; i < v1_neighbours.size(); i++){

		for(int j = 0; j < v2_neighbours.size(); j++){

			if(*v1_neighbours[i] == *v2_neighbours[j])
				count++;
		
		}
	}

	std::cout << "Count common vertices? :  " << count << std::endl;

	return count;

}

void Mesh::collapseVertices(HEVertex* v1, HEVertex* v2){

	std::cout << "Deleted " << std::endl;
	//delete &v1;
	//v1 = v2;

}



void Mesh::collapseRandomEdges(const int faceCnt){

	int removedFaces = 0, removedEdges = 0, removedVertices = 0;

	// As long as too many faces
	//while(HEF.size() > faceCnt){
	for(int i = 0; i < HEF.size(); i++){
		// Select a random edge to remove
		int random_index = rand() % HEE.size();

		//HEEdge* edgeToRemove = HEE[selectedIndex];
		std::set<HEEdge*>::iterator it(HEE.begin());
		advance(it, random_index);

		HEEdge* edgeToRemove = *it;
		HEEdge* edgeToRemoveTwin = edgeToRemove->opposite;

		//std::cout << "edge to REMOVE " << edgeToRemove->id << std::endl;

		HEVertex* hevertex1 = edgeToRemove->hevertex;
		HEVertex* hevertex2 = edgeToRemoveTwin->hevertex;
	
		// Find common vertex (must be exactly two => if so ok to remove)

		//std::cout << "TRYING TO REMOVE EDGE " << HEEToIndex[edgeToRemove] << std::endl;

		if(countCommonVertices(hevertex1, hevertex2) == 2){ 

			// Get faces that share the end point of this edge
			// Get faces that borders this edge

			//collapseVertices(hevertex1, hevertex2);

			//std::cout << "edge to remove vertex " << edgeToRemove->hevertex->vertex << std::endl;

			HEEdge* blue1 = edgeToRemove->next;
			HEEdge* blue2 = edgeToRemove->next->next;
			HEEdge* green1 = blue1->opposite;
			HEEdge* green2 = blue2->opposite;

			HEEdge* blue3 = edgeToRemoveTwin->next;
			HEEdge* blue4 = edgeToRemoveTwin->next->next;
			HEEdge* green3 = blue3->opposite;
			HEEdge* green4 = blue4->opposite;

			//HEEdge* red1 = green1->next->opposite;
			//HEEdge* red2 = green4->next->next;

			// Fix surrounding edges that points to vertex that's removed
			HEEdge* curr = hevertex1->edge->opposite;
			HEEdge* first = hevertex1->edge->opposite;

			do {

			/*	std::cout << "=================================" << std::endl;
				std::cout << "curr vertex" << *curr->hevertex << std::endl;
				std::cout << "he vertex" << *hevertex1 << std::endl;
				std::cout << "edge curr id " << curr->id << std::endl;
				std::cout << "=================================" << std::endl;*/

	
				curr->hevertex = hevertex2;
				curr = curr->next->opposite;

			} while(*curr != *first);

			//if(!(first==edgeToRemove)&&!(first==green1)&&!(first==blue4))

			//delete red1->next->vertex;
			//red1->vertex = hevertex2;
			//red2->vertex = hevertex2;



			// Update merged vertex to not have deleted out edge
			if(*hevertex2->edge == *edgeToRemove||*hevertex2->edge == *blue1){
				hevertex2->edge = blue4->opposite;
				std::cout << "Reassigning remaining vertex outedge" << std::endl;
			}

			green1->opposite = green2;
			green2->opposite = green1;

			green3->opposite = green4;
			green4->opposite = green3;

			//green1->vertex = hevertex2;

			// Saker som inte är med i sliden
			// Update bottom vertex out
			//green2->vertex->edge = green1;
			// Update first green to not point to deleted vertex
			// Update top vertex to not have a deleted out edge
			//green4->vertex->edge = green3;


			

			// Run some tests
			//assert(green4->next->next->opposite == green1->next->opposite->next);
			//assert(green1->next->opposite->vertex == hevertex2);
			//assert(green4->next->next->vertex == hevertex2);
			/*std::cout << "GREEN HAS " << *green1->hevertex << std::endl
					  << "hevertex1" << *hevertex1 << std::endl << std::endl
					  << "hevertex2" << *hevertex2 << std::endl;*/


			for(auto edge : HEE){

				assert(*edge->hevertex != *hevertex1);

			}

			assert(*green4->opposite == *green3);
			assert(*green3->opposite == *green4); 
			assert(*green1->opposite == *green2);
			assert(*green2->opposite == *green1); 
			assert(*hevertex2->edge!=*edgeToRemove||*hevertex2->edge!=*blue1);
			assert(*green1->hevertex != *hevertex1);

			//hevertex2->edge = green4;

			// NOW tryuing to remove 14390

			//std::cout << "ok to remove vertex " << hevertex1->vertex
			//		  << " and " << hevertex2->vertex << std::endl;

			/*std::cout << "Removing vertex id: " << HEVToIndex[hevertex1] << std::endl
					  << "Faces: " << HEFToIndex[edgeToRemove->face] << " and "
					  << HEFToIndex[edgeToRemoveTwin->face] << std::endl
					  << "Edges: " << HEEToIndex[edgeToRemove] << " , "
					  << HEEToIndex[edgeToRemove->next] << " , " 
					  << HEEToIndex[edgeToRemove->next->next] << " , " 
					  << HEEToIndex[edgeToRemoveTwin] << " , "
					  << HEEToIndex[edgeToRemoveTwin->next] << " , "
					  << HEEToIndex[edgeToRemoveTwin->next->next] << std::endl;*/

			// 7854
			// SOMETHING WRONG WITH DELETE, CANT THINK RIGHT NOW
			/*HEV.erase(HEV.begin() + HEVToIndex[hevertex1] - removedVertices);
			removedVertices++;
			HEF.erase(HEF.begin() + HEFToIndex[edgeToRemove->face] - removedFaces);
			removedFaces++;
			HEF.erase(HEF.begin() + HEFToIndex[edgeToRemoveTwin->face] - removedFaces);
			removedFaces++;
			HEE.erase(HEE.begin() + HEEToIndex[edgeToRemove] - removedEdges);
			removedEdges++;
			HEE.erase(HEE.begin() + HEEToIndex[blue1] - removedEdges);
			removedEdges++;
			HEE.erase(HEE.begin() + HEEToIndex[blue2] - removedEdges);
			removedEdges++;
			HEE.erase(HEE.begin() + HEEToIndex[edgeToRemoveTwin] - removedEdges);
			removedEdges++;
			HEE.erase(HEE.begin() + HEEToIndex[blue3] - removedEdges);
			removedEdges++;
			HEE.erase(HEE.begin() + HEEToIndex[blue4] - removedEdges);
			removedEdges++;*/

			HEV.erase(hevertex1);

			HEF.erase(edgeToRemove->face);
			HEF.erase(edgeToRemoveTwin->face);

			HEE.erase(edgeToRemove);
			HEE.erase(blue1);
			HEE.erase(blue2);
			HEE.erase(edgeToRemoveTwin);
			HEE.erase(blue3);
			HEE.erase(blue4);

			std::cout << "HEF size" << HEF.size() << std::endl;


		} else {

			std::cout << "not deleted" << std::endl;
		}

		

	}

}
	
// faceCnt = max number of vertices
void Mesh::simplifyMesh(const char* input, const char* output, int faceCnt){
	// you may assume inputs are always valid
	loadMF(input);
	convertMesh();
	std::cout<<"Original face count: "<<HEF.size()<<std::endl;
	// do mesh simplification here
	collapseRandomEdges(faceCnt);
	revertMesh();
	writeMF(output);
}

/* Convert the mesh into the half-edge data structure*/
void Mesh::convertMesh(){

	std::map<std::pair<uint,uint>, HEEdge*> edges; // Keep track of opposite edges
	std::vector<HEVertex*> added_hevertices;
	int edge_index = 0;

	// Add vertices
	for(int i = 0; i < V.size(); i++){
		HEVertex* _hevertex = new HEVertex();
		_hevertex->vertex = V[i];

		//HEVToIndex[_hevertex] = i;
		added_hevertices.push_back(_hevertex);
		HEV.insert(_hevertex);
	}

	// Go through all faces
	for(int i = 0; i < F.size(); i++){

		// Declare the variables,
		// Assume that the vertices are declared in a clockwise order
		HEFace* _heface = new HEFace();

		HEEdge* _halfedge1 = new HEEdge();
		HEEdge* _halfedge2 = new HEEdge();
		HEEdge* _halfedge3 = new HEEdge();

		HEVertex* _hevertex1 = added_hevertices[F[i].a-1];
		HEVertex* _hevertex2 = added_hevertices[F[i].b-1];
		HEVertex* _hevertex3 = added_hevertices[F[i].c-1];

		// HEFace
		_heface->edge = _halfedge1; 

		// Update pointers from halfedge to vertex 
		_halfedge3->hevertex = _hevertex1;
		_hevertex1->edge = _halfedge1;

		//_hevertex2->edge = _halfedge2;
		_hevertex2->edge = _halfedge3;
		_halfedge2->hevertex = _hevertex2;

		//_hevertex3->edge = _halfedge3;
		_hevertex3->edge = _halfedge2;
		_halfedge1->hevertex = _hevertex3;

		// Next
		_halfedge1->next = _halfedge2;
		_halfedge2->next = _halfedge3;
		_halfedge3->next = _halfedge1;

		// Face
		_halfedge1->face = _heface;
		_halfedge2->face = _heface;
		_halfedge3->face = _heface;

		edges[std::make_pair(F[i].b, F[i].a)] = _halfedge3;
		edges[std::make_pair(F[i].c, F[i].b)] = _halfedge2;
		edges[std::make_pair(F[i].a, F[i].c)] = _halfedge1;

		// If opposite edge has been added
		if(edges.find(std::make_pair(F[i].a, F[i].b)) != edges.end()){

			edges[std::make_pair(F[i].b, F[i].a)]->opposite = edges[std::make_pair(F[i].a, F[i].b)];
			edges[std::make_pair(F[i].a, F[i].b)]->opposite = edges[std::make_pair(F[i].b, F[i].a)];

		}

		if(edges.find(std::make_pair(F[i].b, F[i].c)) != edges.end()){

			edges[std::make_pair(F[i].c, F[i].b)]->opposite = edges[std::make_pair(F[i].b, F[i].c)];
			edges[std::make_pair(F[i].b, F[i].c)]->opposite = edges[std::make_pair(F[i].c, F[i].b)];

		} 

		if(edges.find(std::make_pair(F[i].c, F[i].a)) != edges.end()){

			edges[std::make_pair(F[i].a, F[i].c)]->opposite = edges[std::make_pair(F[i].c, F[i].a)];
			edges[std::make_pair(F[i].c, F[i].a)]->opposite = edges[std::make_pair(F[i].a, F[i].c)];

		} 

		//HEF.push_back(_heface);
		HEF.insert(_heface);

		//HEFToIndex[_heface] = i;

	/*	HEE.push_back(_halfedge1);
		HEE.push_back(_halfedge2);
		HEE.push_back(_halfedge3);*/

		_halfedge1->id = edge_index++;
		_halfedge2->id = edge_index++;
		_halfedge3->id = edge_index++;

		HEE.insert(_halfedge1);
		HEE.insert(_halfedge2);
		HEE.insert(_halfedge3);
	
		//if(edge_index < 50)
		//	std::cout << "EDGE INSERTED INDEX BEFORE " << edge_index << std::endl;

	/*	HEEToIndex[_halfedge1] = edge_index;
		edge_index++;
		HEEToIndex[_halfedge2] = edge_index;
		edge_index++;
		HEEToIndex[_halfedge3] = edge_index;
		edge_index++;*/
		
		//if(edge_index < 50)
		//	std::cout << "EDGE INSERTED INDEX AFTER " << edge_index << std::endl;
				
	}

	for(auto edge : HEE){
		assert(*edge->opposite == *(*HEE.find(edge))->opposite);
		//assert(*edge->opposite == *(*it)->opposite);
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

	/*for(int i = 0; i < HEV.size(); i++){

		Vertex v = {};
		v = HEV[i]->vertex;
		V.push_back(v);
		indices[*HEV[i]] = i+1;
	}*/

	int tempindex = 1;

	std::set<HEVertex*>::iterator it;

	for(it = HEV.begin(); it != HEV.end(); ++it){

		HEVertex* he_vertex = *it;
		Vertex v;
		v = (*it)->vertex;
		V.push_back(v);
		indices[**it] = tempindex++;

	}

	/*

	for( auto he_vertex : HEV ){

		Vertex v;
		v = he_vertex->vertex;
		V.push_back(v);
		indices[*he_vertex] = tempindex++;

	}*/

	assert(V.size() == HEV.size());	

	for( auto heface : HEF ) {

		Face f;

		HEVertex* _hevertex3 = heface->edge->hevertex;
		HEVertex* _hevertex2 = heface->edge->next->hevertex;
		HEVertex* _hevertex1 = heface->edge->next->next->hevertex;

		f.a = indices[*_hevertex1];
		f.b = indices[*_hevertex2];
		f.c = indices[*_hevertex3];

		F.push_back(f);

	}

	/*for(int i = 0; i < HEF.size(); i++){

		Face f;

		HEVertex* _hevertex3 = HEF[i]->edge->vertex;
		HEVertex* _hevertex2 = HEF[i]->edge->next->vertex;
		HEVertex* _hevertex1 = HEF[i]->edge->next->next->vertex; 
		
		f.a = indices[*_hevertex1];
		f.b = indices[*_hevertex2];
		f.c = indices[*_hevertex3];

		// No elements in face can be equal
		assert(f.a != f.b && f.b != f.c && f.c != f.a);

		F.push_back(f);

	}*/

	assert(HEF.size() == F.size());


}


std::vector<HEFace*> Mesh::neighborFaces(HEVertex* v){
	return std::vector<HEFace*>();
}

std::vector<HEVertex*> Mesh::adjacentVertices(HEFace* f){
	return std::vector<HEVertex*>();
}

int Mesh::Vcnt(){
	return HEV.size();
}

int Mesh::Fcnt(){
	return HEF.size();
}