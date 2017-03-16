/////////////////////////////////////////////////////////////////////
// StrongComponent.cpp - Find Strong Components in dependency      //
//                       graph                                     //
//                                                                 //
// Application: CSE687 - Object Oriented Design Project #2         //
// Language:    C++, Visual Studio 2015                            //
// Platform:    Win 8.1, Visual Studio 2015                        //
// Author:      WENYU ZHANG, 233508014                             //
//              wzhan102@syr.edu                                   //
/////////////////////////////////////////////////////////////////////

#include "StrongComponent.h"

using namespace CodeAnalysis;

//----< promotion constructor >-------------------------------------------------------

StrongComp::StrongComp(Database db) {
	generateGraph(db);
	edgeCount_ = 1;
	timeStamp_ = 0;
	initVectors();
}

//----< promotion constructor given graph >-------------------------------------------

StrongComp::StrongComp(Graph graph) 
	:graph_(graph)
{
	edgeCount_ = 1;
	timeStamp_ = 0;
	initVectors();
}

//----< display graph contents >------------------------------------------------------

void StrongComp::showGraph() {
	Display::show(graph_);
	std::cout << "\n";
}

//----< find strong components >------------------------------------------------------

void StrongComp::findComps() {
	for (size_t i = 0; i < graph_.size(); ++i) {
		if (disc_[i] == -1) {
			tarjan(graph_[graph_.findVertexIndexById(i)]);
		}
	}
}

//----< generate SCC by vertex's value >-----------------------------------------------

StrongComp::StrComponents StrongComp::generateSCC() {
	StrComponents strComp;
	for (auto comp : comps_) {
		std::vector<std::string> compTemp;
		for (auto vertex : comp) {
			vertex.value();
			compTemp.push_back(vertex.value());
		}
		strComp.push_back(compTemp);
	}
	return strComp;
}

//----< display strong connected components >-----------------------------------------

std::string StrongComp::showComponents() {
	std::ostringstream out;
	int count = 1;
	for (auto comp : comps_) {
		out << "\n -----< Strong Connected Component #" << count << " >-----";
		for (auto vertex : comp) {
			out << vertex.value() << " ";
		}
		++count;
	}
	return out.str();
}

//----< generate graph by given database >--------------------------------------------

void StrongComp::generateGraph(Database db) {
	// fullfill vertes and vertex map
	for (auto file : db.keySet()) {
		Vertex* vertex = new Vertex(file);
		graph_.addVertex(*vertex);
		map_[file] = vertex;
	}
	// add all edges
	for (auto file : db.keySet()) {
		for (auto child : db[file][child]) {
			std::string edgeName = "e" + edgeCount_;
			graph_.addEdge(edgeName, *map_[file], *map_[child]);
			edgeCount_++;
		}
	}
}

//----< initialize private vectors >--------------------------------------------------

void StrongComp::initVectors() {
	//std::cout << "\nGraph size: " << graph_.size() << std::endl;
	for (size_t i = 0; i < graph_.size(); i++) {
		disc_.push_back(-1);
		low_.push_back(-1);
		check_.push_back(false);
	}
}

//----< perform Tarjan's algorithm >--------------------------------------------------

void StrongComp::tarjan(Vertex v) {
	disc_[v.id()] = low_[v.id()] = ++timeStamp_;
	stack_.push(v.id());
	check_[v.id()] = true;
	// Traverse all connected vertes
	for (size_t i = 0; i < v.size(); ++i)
	{
		Vertex::Edge edge = v[i];
		size_t adjId = graph_[edge.first].id();
		if (disc_[adjId] == -1) {
			tarjan(graph_[edge.first]);
			low_[v.id()] = std::min(low_[v.id()], low_[adjId]);
		}
		else if (check_[adjId] == true)
			low_[v.id()] = std::min(low_[v.id()], disc_[adjId]);
	}
	// Found head node, pop the stack
	size_t w = 0;  // To store stack extracted vertices
	if (low_[v.id()] == disc_[v.id()])
	{
		while (stack_.top() != v.id())
		{
			w = stack_.top();
			// add to current SCC
			currComp_.push_back(graph_[graph_.findVertexIndexById(w)]);
			check_[w] = false;
			stack_.pop();
		}
		w = stack_.top();
		// output current SCC
		currComp_.push_back(graph_[graph_.findVertexIndexById(w)]);
		comps_.push_back(currComp_);
		currComp_.clear();
		check_[w] = false;
		stack_.pop();
	}
}

//----< test stub >-------------------------------------------------------------------

#ifdef TEST_STRONGCOMP

using Vertex = GraphLib::Vertex<std::string, std::string>;
using Graph = GraphLib::Graph<std::string, std::string>;
using Display = GraphLib::Display<std::string, std::string>;

//----< generate graph instance >-----------------------------------------------------

Graph graphInstance() {
	Graph g;
	Vertex v1("v1");
	Vertex v2("v2");
	Vertex v3("v3");
	Vertex v4("v4");
	Vertex v5("v5");
	Vertex v6("v6");
	g.addVertex(v1);
	g.addVertex(v2);
	g.addVertex(v3);
	g.addVertex(v4);
	g.addVertex(v5);
	g.addVertex(v6);
	g.addEdge("e1", v1, v2);
	g.addEdge("e2", v1, v3);
	g.addEdge("e3", v2, v4);
	g.addEdge("e4", v3, v4);
	g.addEdge("e5", v4, v1);
	g.addEdge("e6", v3, v5);
	g.addEdge("e7", v4, v6);
	g.addEdge("e8", v5, v6);
	return g;
}

int main() 
{
	try
	{
		std::cout << "\n  Constructing Graph instance";
		std::cout << "\n -----------------------------";
		Graph graph = graphInstance();
		Display::show(graph);
		std::cout << std::endl;

		std::cout << "\n  Constructing StrongComp instance";
		std::cout << "\n ----------------------------------";
		StrongComp sc(graph);

		std::cout << "\n  Find Strong Connected Components";
		std::cout << "\n ----------------------------------";
		sc.findComps();
		std::cout << std::endl;
		std::cout << sc.showComponents();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
	std::cout << "\n\n";
	return 0;
}

#endif // !TEST_STRONGCOMP
