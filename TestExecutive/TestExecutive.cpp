/////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Test code for Project 2                     //
//                                                                 //
// Application: CSE687 - Object Oriented Design Project #2         //
// Language:    C++, Visual Studio 2015                            //
// Platform:    Win 8.1, Visual Studio 2015                        //
// Author:      WENYU ZHANG, 233508014                             //
//              wzhan102@syr.edu                                   //
/////////////////////////////////////////////////////////////////////
#include "TestExecutive.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <chrono>
#include <ctime>

#include "../Parser/Parser.h"
#include "../FileSystem/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../TypeAnalysis/TypeAnalyzer.h"
#include "../TypeTable/TypeTable.h"
#include "../Display/Display.h"
#include "../Persist/Persist.h"

using Rslt = Logging::StaticLogger<0>;  // use for application results
using Demo = Logging::StaticLogger<1>;  // use for demonstrations of processing
using Dbug = Logging::StaticLogger<2>;  // use for debug output

using namespace CodeAnalysis;

//----< destructor for TestExecutive >------------------------------------------------

TestExecutive::~TestExecutive() {
	delete depAnal;
	delete db;
}

//----< test Requirement #2 >---------------------------------------------------------

void TestExecutive::testRequirement2() {
	makeTitle("Test Requirement #2");
	makeComment("This project only use C++ standard library's stream for all I/O.");
	makeComment("All Heap-based memory are approriately managed.");
	std::cout << std::endl;
}

//----< test Requirement #3 >---------------------------------------------------------

void TestExecutive::testRequirement3() {
	makeTitle("Test Requirement #3");
	makeComment("This project contains all required packages described in Purpose section");
	makeSubTitle("Required Package List:");
	std::cout << "\n    " << std::setw(9) << "TypeTable";
	std::cout << "\n    " << std::setw(10) << "TypeAnalysis";
	std::cout << "\n    " << std::setw(10) << "DependencyAnalysis";
	std::cout << "\n    " << std::setw(10) << "StrongComponent";
	std::cout << "\n    " << std::setw(7) << "Display";
	std::cout << "\n    " << std::setw(10) << "NoSqlDatabase";
	std::cout << "\n    " << std::setw(10) << "TestExecutive";
	std::cout << "\n\n";
}

//----< test TypeAnalysis >-----------------------------------------------------------

void TestExecutive::typeAnalysis() {
	makeTitle("Test Requirement #4: Type Analysis");
	// Test the TypeTable after type analysis
	TypeAnalyzer ta(table_);
	ta.startAnalysis();
	std::cout << std::endl;
	makeSubTitle("Display TypeTable Contents after Analyzing");
	std::cout << std::endl;
	std::cout << table_.showTable();
}

//----< test Dependency Analysis >----------------------------------------------------

void TestExecutive::depAnalysis() {
	makeTitle("Test Requirement #5: Dependency Analysis");
	depAnal = new DepAnalyzer(cppHeaderFiles(), cppImplemFiles(), table_);
	depAnal->processHFiles();
	depAnal->processImpleFiles();
	db = new NoSqlDatabase::NoSqlDB<std::string>(depAnal->getResult());
	std::cout << std::endl;
	makeSubTitle("Display Dependency Relationships");
	std::cout << std::endl;
	makeComment("In database, <Key> represents fileName, <ChildCollection> represents dependency.");
	std::cout << std::endl;
	std::cout << depAnal->showDependencies() << std::endl;
}

//----< test Strong Component Analysis >----------------------------------------------

void TestExecutive::strongComp() {
	makeTitle("Test Requirement #6: Strong Connected Component");
	sc = new StrongComp(*db);
	makeSubTitle("Generated Dependency Graph:");
	sc->showGraph();
	sc->findComps();
	strComp_ = sc->generateSCC();
	makeSubTitle("Display Strong Connected Components:");
	std::cout << showSCC() << std::endl;
}

//----< test Requirement #7 >---------------------------------------------------------

void TestExecutive::testRequirement7() {
	std::cout << std::endl;
	makeTitle("Test Requirement #7: Generate Xml File");
	std::cout << std::endl;
	NoSqlDatabase::Persist persist;
	if (outPath == "")
		outPath = "./analysis_results.xml";
	makeSubTitle("Display Dependency Analysis Result in Xml:");
	Xml depXml = depToXml();
	std::cout << depXml << std::endl;
	makeSubTitle("Display Strong Component Analysis Result in Xml:");
	std::cout << std::endl;
	Xml sccXml = sccToXml();
	std::cout << sccXml << std::endl;

	std::ostringstream out;
	out << "========== Dependency Relationship ==========\n" << depXml << std::endl;
	out << "\n========== Strong Components ==========\n" << sccXml << std::endl;
	persist.setOutPath(outPath);
	persist.save(out.str());
	makeComment("Xml file succefully saved to directory: " + outPath);
	std::cout << std::endl;
}

//----< test Requirement #8 >---------------------------------------------------------

void TestExecutive::testRequirement8() {
	makeTitle("Test Requirement #8: Process Command Line");
	makeSubTitle("Command Line Accepts 3 arguments");
	std::cout << std::endl;
	std::cout << "\n 1. Path to the directory tree containing files to analyze.\n";
	std::cout << "    This desmonstration uses \"./TestFolder\" which contains test files.";
	std::cout << std::endl;
	std::cout << "\n 2. List of file patterns to match for selection of files to analyze.\n";
	std::cout << "    This desmonstration uses \"*.h, *.cpp\".";
	std::cout << std::endl;
	std::cout << "\n 3. <Optional> Specification of XML results file, which should be a valid path ending with \".xml\".\n";
	std::cout << "    By default saved to \"project2/analysis_results.xml\"." << std::endl;
	std::cout << "    File would save to default file when input path is unvalid." << std::endl;
}

//----< test Requirement #9 >---------------------------------------------------------

void TestExecutive::testRequirement9() {
	std::cout << std::endl;
	makeTitle("Test Requirement #9: Process Command Line");
	makeComment("Requirements #2 -- #8 already demonstrated above");
}

//----< display strong components contents >------------------------------------------

std::string TestExecutive::showSCC() {
	std::ostringstream out;
	int count = 1;
	for (auto comp : strComp_) {
		out << "\n\n  -----< Strong Connected Component #" << count << " >-----";
		for (auto elem : comp) {
			out << "\n  " << elem;
		}
		++count;
	}
	return out.str();
}

//----< write dependency analysis result to xml >-------------------------------------

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;
TestExecutive::Xml TestExecutive::depToXml() {
	XmlProcessing::XmlDocument doc;
	NoSqlDatabase::sPtr pRoot = makeTaggedElement("Dependency-Relationship");
	doc.docElement() = pRoot;
	for (auto fileName : db->keySet()) {
		sPtr file = makeTaggedElement("file");
		sPtr filename = makeTaggedElement("file-name");
		filename->addChild(makeTextElement(fileName));
		file->addChild(filename);
		pRoot->addChild(file);
		sPtr deps = makeTaggedElement("dependencies");
		file->addChild(deps);
		for (auto child : (*db)[fileName][child]) {
			sPtr depFile = makeTaggedElement("dependency");
			depFile->addChild(makeTextElement(child));
			deps->addChild(depFile);
		}
	}
	return doc.toString();
}

//----< write SCCs analysis result to xml >-------------------------------------------

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;
TestExecutive::Xml TestExecutive::sccToXml() {
	XmlProcessing::XmlDocument doc;
	NoSqlDatabase::sPtr pRoot = makeTaggedElement("Strong-Components-Result");
	doc.docElement() = pRoot;
	for (auto comps : strComp_) {
		sPtr scc = makeTaggedElement("strong-components");
		pRoot->addChild(scc);
		sPtr files = makeTaggedElement("files");
		scc->addChild(files);
		for (auto elem : comps) {
			sPtr filename = makeTaggedElement("file-name");
			filename->addChild(makeTextElement(elem));
			files->addChild(filename);
		}
	}
	return doc.toString();
}

//----< process command line >--------------------------------------------------------

bool TestExecutive::processCommands(int argc, char* argv[]) {
	try {
		outPath = argv[argc - 1];
		if (outPath.find(".xml") == std::string::npos)
			outPath = "";
		return true;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  command line argument parsing error:";
		std::cout << "\n  " << ex.what() << "\n\n";
		return false;
	}
	return true;
}

//----< test stub >-------------------------------------------------------------------

#ifdef TEST_EXECUTIVE

#include <fstream>

int main(int argc, char* argv[])
{
	using namespace CodeAnalysis;

	TypeTable testTable;
	TestExecutive exec(testTable);

	try {
		bool succeeded = exec.ProcessCommandLine(argc, argv);
		if (!succeeded) { return 1; }
		exec.processCommands(argc, argv);
		exec.setDisplayModes();
		exec.startLogger(std::cout);
		exec.getSourceFiles();
		exec.processSourceCode(true);
		exec.complexityAnalysis();
		exec.dispatchOptionalDisplays();
		exec.flushLogger();
		exec.stopLogger();
		exec.testRequirement2();
		exec.testRequirement3();
		exec.typeAnalysis();
		exec.depAnalysis();
		exec.strongComp();
		exec.testRequirement7();
		exec.testRequirement8();
		exec.testRequirement9();
	}
	catch (std::exception& except)
	{
		exec.flushLogger();
		std::cout << "\n\n  caught exception in Executive::main: " + std::string(except.what()) + "\n\n";
		exec.stopLogger();
		return 1;
	}
	return 0;
}

#endif // !TEST_EXECUTIVE