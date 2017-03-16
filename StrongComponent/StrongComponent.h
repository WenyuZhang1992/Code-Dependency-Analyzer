#ifndef STRONGCOMPONENT_H
#define STRONGCOMPONENT_H
/////////////////////////////////////////////////////////////////////
// StrongComponent.h - Find Strong Components in dependency graph  //
//                                                                 //
// Application: CSE687 - Object Oriented Design Project #2         //
// Language:    C++, Visual Studio 2015                            //
// Platform:    Win 8.1, Visual Studio 2015                        //
// Author:      WENYU ZHANG, 233508014                             //
//              wzhan102@syr.edu                                   //
/////////////////////////////////////////////////////////////////////
/*
*  Class Operations
* ====================
*  - This package contains one class: StrongComp
*  - StrongComp: Find strong connected components on directed graph
*  - Provide method to build graph given NoSql database
*  - Find strong connected component using Tarjan's algorithm
*
*  Public Interface
* ==================
*  StrongComp sc(Database db)                         create StrongComp instance by NoSql database
*  StrongComp sc1(Graph graph)                        create StrongComp instance by graph instance
*  sc.findComps()                                     find SCCs and store in private attribute
*  sc.generateSCC()                                   return SCCs as std::vector
*  sc.showGraph()                                     display the contents of graph_
*  sc.showComponents()                                display SCCs in string
*
/////////////////////////////////////////////////////////////////////
// Build Process                                                   //
/////////////////////////////////////////////////////////////////////
//  Required Files:                                                //
//  - StrongComponent.h, StrongComponent.cpp, Graph.h              // 
//    NoSqlDatabase.h                                              //
//                                                                 //
//  Build Process:                                                 //
//    Set preprocessor definition TEST_STRONGCOMP in properties.   //
//    Set StrongComponent as the startup project.                  //
//    Compile and run in Visual Studio.                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////
*
*  Build Command
* ===============
*    devenv StrongComponent.sln /rebuild debug
*
*  Maintenance History
* =====================
*  ver 1.0 : 16 Mar 17
*  - first release
*
*/

#include <unordered_map>
#include <vector>
#include <stack>
#include "../Graph/Graph.h"
#include "../NoSqlDatabase/NoSqlDatabase.h"

namespace CodeAnalysis {
	
	///////////////////////////////////////////////////////////////////////////
	// StrongComp:
	// - Find Strong Components in dependency graph

	class StrongComp {
		
	public:
		using Database = NoSqlDatabase::NoSqlDB<std::string>;
		using Vertex = GraphLib::Vertex<std::string, std::string>;
		using Graph = GraphLib::Graph<std::string, std::string>;
		using Display = GraphLib::Display<std::string, std::string>;
		using VertexMap = std::unordered_map<std::string, Vertex*>;
		using Stack = std::stack<size_t>;
		using Vector = std::vector<int>;
		using BVector = std::vector<bool>;
		using Component = std::vector<Vertex>;
		using Components = std::vector<Component>;
		using StrComponents = std::vector<std::vector<std::string>>;

		StrongComp(Database db);                                // promotion constructor
		StrongComp(Graph graph);                                // promotion constructor
		~StrongComp() {}                                        // destructor
		void findComps();                                       // find strong componnets
		void showGraph();                                       // display graph contents
		StrComponents generateSCC();                            // generate SCC in string format
		std::string showComponents();                           // display the strong connected components

	private:

		void generateGraph(Database db);                        // generate graph by given database
		void initVectors();                                     // initialize private vectors
		void tarjan(Vertex v);                                  // conduct Tarjan's algorithm
		Graph graph_;
		VertexMap map_;
		size_t edgeCount_;
		Component currComp_;
		Components comps_;
		Stack stack_; 
		Vector disc_;
		Vector low_;
		BVector check_;
		int timeStamp_;
	};
}



#endif // !STRONGCOMPONENT_H


