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

	if(!v){
		std::cout << "Vertex is null, no neighbours" << std::endl;
		return tmp;
	}


	//HEEdge* first = v->edge;
	//HEEdge* curr = v->edge->opposite->next;
	HEEdge* curr = v->edge;
	HEEdge* first = v->edge;

	std::cout << "does first exits " << first->exists << std::endl;

	if(!first || !first->exists){
		std::cout << "Invalid edge, no neighbours" << std::endl;
		return tmp;
	} else {

		std::cout << "first does exists with id " << first->id << std::endl;

	}

	//std::cout << "BEFORE LOOP FIRST " << first->id << std::endl;
	//std::cout << "BEFORE LOOP vertex " << *first->hevertex << std::endl;

	int count = 0;

	do{

		tmp.push_back(curr->hevertex);
		curr = curr->opposite->next;

		/*if(count > 20 ){
			std::cout << " IN CHECK STUCK AT " << first->id << std::endl;
		}
		count++;*/

	} while(*curr != *first);

	//std::cout << "DONE MOTHERFUCKER " << std::endl;

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

bool Mesh::canEdgeCollapse(HEEdge* _heedge){

	if(_heedge && _heedge->exists && countCommonVertices(_heedge->hevertex, _heedge->opposite->hevertex) == 2){
		return true;
	} else {
		return false;
	}

}

void Mesh::debugAdjacentEdges(){

	for(auto v : HEV){

		//HEEdge* curr = v->edge->opposite->next;
		HEEdge* curr = v->edge;
		HEEdge* first = v->edge;

		if(!first->exists)
			continue;

		do{

			curr = curr->opposite->next;

			/*if(count > 20 ){
				
				std::cout << "edge " << first->id << " is STUCK!" << std::endl;

			}

			count++;*/

		} while(*curr != *first);

	}

}

void Mesh::collapseRandomEdges(const int faceCnt){

	int removedFaces = 0, removedEdges = 0, removedVertices = 0;

	// As long as too many faces
	while(HEF.size() > faceCnt){
	//for(int i = 0; i < HEF.size(); i++){
		// Select a random edge to remove
		int random_index = rand() % HEE.size();

		std::set<HEEdge*>::iterator it(HEE.begin());
		advance(it, random_index);

		HEEdge* edgeToRemove = *it;

		if(canEdgeCollapse(edgeToRemove)){ 

			std::cout << "** Can remove edge with id " << edgeToRemove->id << std::endl;

			HEEdge* edgeToRemoveTwin = edgeToRemove->opposite;
			HEVertex* hevertex1 = edgeToRemove->hevertex;
			HEVertex* hevertex2 = edgeToRemoveTwin->hevertex;

			// Get faces that share the end point of this edge
			// Get faces that borders this edge

			HEEdge* e_next = edgeToRemove->next;
			HEEdge* e_prev = edgeToRemove->next->next;
			HEEdge* e_next_twin = e_next->opposite;
			HEEdge* e_prev_twin = e_prev->opposite;

			HEEdge* et_next = edgeToRemoveTwin->next;
			HEEdge* et_prev = edgeToRemoveTwin->next->next;
			HEEdge* et_next_twin = et_next->opposite;
			HEEdge* et_prev_twin = et_prev->opposite;

			// Fix surrounding edges that points to vertex that's removed
			HEEdge* curr = hevertex1->edge->opposite;
			HEEdge* first = hevertex1->edge->opposite;

			// Reassign all incoming vertices from removed one to next
			do {
				curr->hevertex = hevertex2;
				curr = curr->next->opposite;
			} while(*curr != *first);

			if(*hevertex2->edge == *edgeToRemove||*hevertex2->edge == *e_next){
				hevertex2->edge = et_prev->opposite;
				std::cout << "** Reassignment of remaining vertex outedge" << std::endl;
			}

			// Bind together these
			e_next_twin->opposite = e_prev_twin;
			e_prev_twin->opposite = e_next_twin;
			
			et_next_twin->opposite = et_prev_twin;
			et_prev_twin->opposite = et_next_twin;
			
			for(auto edge : HEE){
				assert(*edge->hevertex != *hevertex1);
			}

			// Run some tests
			assert(*et_prev_twin->opposite == *et_next_twin);
			assert(*et_next_twin->opposite == *et_prev_twin); 
			assert(*e_next_twin->opposite == *e_prev_twin);
			assert(*e_prev_twin->opposite == *e_next_twin); 
			assert(*hevertex2->edge!=*edgeToRemove||*hevertex2->edge!=*e_next);
			assert(*e_next_twin->hevertex != *hevertex1);

			HEV.erase(hevertex1);

			HEF.erase(edgeToRemove->face);
			HEF.erase(edgeToRemoveTwin->face);

			HEE.erase(edgeToRemove);
			HEE.erase(e_next);
			HEE.erase(e_prev);
			HEE.erase(edgeToRemoveTwin);
			HEE.erase(et_next);
			HEE.erase(et_prev);

/*			delete hevertex1;

			delete edgeToRemove->face;
			delete edgeToRemoveTwin->face;

			delete edgeToRemoveTwin;
			delete edgeToRemove;

			delete e_next;
			delete e_prev;
			delete et_next;
			delete et_prev;*/

			//hevertex1->exists = false;

			//edgeToRemove->face->exists = false;
			//edgeToRemoveTwin->face->exists = false;

			edgeToRemove->exists = false;
			e_next->exists = false;
			e_prev->exists = false;
			edgeToRemoveTwin->exists = false;
			et_next->exists = false;
			et_prev->exists = false;

			//debugAdjacentEdges();

			std::cout << "** HEF size" << HEF.size() << std::endl;

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

		_halfedge1->exists = true;
		_halfedge2->exists = true;
		_halfedge3->exists = true;

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


		_halfedge1->id = edge_index++;
		_halfedge2->id = edge_index++;
		_halfedge3->id = edge_index++;

		HEE.insert(_halfedge1);
		HEE.insert(_halfedge2);
		HEE.insert(_halfedge3);

		assert(*_halfedge1->next == *_halfedge2);
		assert(*_halfedge2->next == *_halfedge3);
		assert(*_halfedge3->next == *_halfedge1);

		assert(*_halfedge1->next->next == *_halfedge3);
		assert(*_halfedge2->next->next == *_halfedge1);
		assert(*_halfedge3->next->next == *_halfedge2);
				
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

	int tempindex = 1;

	std::set<HEVertex*>::iterator it;

	for(it = HEV.begin(); it != HEV.end(); ++it){

		HEVertex* he_vertex = *it;
		Vertex v;
		v = (*it)->vertex;
		V.push_back(v);
		indices[**it] = tempindex++;

	}

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