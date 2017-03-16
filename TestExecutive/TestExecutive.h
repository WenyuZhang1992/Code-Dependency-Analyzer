#ifndef TESTEXECUTIVE_H
#define TESTEXECUTIVE_H
/////////////////////////////////////////////////////////////////////
// TestExecutive.h - Test code for Project 2                       //
//                                                                 //
// Application: CSE687 - Object Oriented Design Project #2         //
// Language:    C++, Visual Studio 2015                            //
// Platform:    Win 8.1, Visual Studio 2015                        //
// Author:      WENYU ZHANG, 233508014                             //
//              wzhan102@syr.edu                                   //
/////////////////////////////////////////////////////////////////////
/*
*  Module Operations
* ====================
*  - TestExecutive inherits from Executive class, extends
*    the base class functionalities to direct and organize
*    Type Analysis, Dependency Analysis and Strong Component Analysis
*  - TestExecutive provides demonstration for requirements of
*    project2.
*  - TestExecutive provides methods to process command line arguments
*
*  Public Interface
* ==================
*  TestExecutive exec(TypeTable&)                      create a TestExecutive instance
*  exec.testRequirement2()                             demonstrate Requirement #2
*  exec.testRequirement3()                             demonstrate Requirement #3
*  exec.typeAnalysis()                                 conduct type analysis
*  exec.depAnalysis()                                  conduct dependency analysis
*  exec.strongComp()                                   conduct strong component analysis  
*  exec.testRequirement7()                             demonstrate Requirement #7
*  exec.testRequirement8()                             demonstrate Requirement #8
*  exec.testRequirement9()                             demonstrate Requirement #9
*  exec.processCommands(int argc, char* argv[])        process command line arguments
*
/////////////////////////////////////////////////////////////////////
// Build Process                                                   //
/////////////////////////////////////////////////////////////////////
//  Required Files:                                                //
//  - TestExecutive.h, TestExecutive.cpp, Executive.h              //
//    TypeAnalyzer.h, DependencyAnalyzer.h, TypeTable.h            //
//    NoSqlDatabase.h, StrongComponent.h, XmlDocument.h            //
//                                                                 //
//  Build Process:                                                 //
//    Set preprocessor definition TEST_EXECUTIVE in properties.    //
//    Set TestExecutive as the startup project.                    //
//    Compile and run in Visual Studio.                            //
//                                                                 //
/////////////////////////////////////////////////////////////////////
*
*  Build Command
* ===============
*    devenv TestExecutive.sln /rebuild debug
*
*  Maintenance History
* =====================
*  ver 1.0 : 16 Mar 17
*  - first release
*
*/

#include "../Executive/Executive.h"
#include "../TypeTable/TypeTable.h"
#include "../TypeAnalysis/TypeAnalyzer.h"
#include "../DependencyAnalysis/DependencyAnalyzer.h"
#include "../NoSqlDatabase/NoSqlDatabase.h"
#include "../StrongComponent/StrongComponent.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"

namespace CodeAnalysis {
	
	///////////////////////////////////////////////////////////////////////////
	// TestExecutive:
	// - Provide executive code for type analysis and dependency analysis
	// - Provide demonstration for project

	class TestExecutive : public CodeAnalysisExecutive {
	public:

		using Database = NoSqlDatabase::NoSqlDB<std::string>;
		using Vertex = GraphLib::Vertex<std::string, std::string>;
		using Graph = GraphLib::Graph<std::string, std::string>;
		using StrComponents = std::vector<std::vector<std::string>>;
		using Xml = std::string;
		using Path = std::string;

		TestExecutive(TypeTable& table) : table_(table) {}        // promotion constructor
		~TestExecutive();                                         // destructor

		void testRequirement2();                                  // test code for Requirement 2
		void testRequirement3();                                  // test code for Requirement 3
		void typeAnalysis();                                      // conduct type analysis
		void depAnalysis();                                       // conduct dependency analysis
		void strongComp();                                        // test strong component analysis
		void testRequirement7();                                  // test code for Requirement 7
		void testRequirement8();                                  // test code for Requirement 8
		void testRequirement9();                                  // test code for Requirement 9
		bool processCommands(int argc, char* argv[]);             // process command line

	private:

		std::string showSCC();                                    // display strong components
		Xml depToXml();                                           // write dependency analysis result to xml
		Xml sccToXml();                                           // write strong component analysis result to xml
		TypeTable& table_;
		DepAnalyzer* depAnal;
		Database *db;
		Graph *graph;
		StrongComp *sc;
		std::vector<std::vector<std::string>> strComp_;
		Path outPath;

	};
}
#endif // !TESTEXECUTIVE_H


