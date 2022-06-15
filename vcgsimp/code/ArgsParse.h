#pragma once
#include<string>
#include<iostream>
#include<vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace OPT {
	std::string strInputMeshFileName;
	std::string strOutputFileName;
	std::string Multiple;
	std::string strOutputDirName;
	boost::program_options::variables_map vm;
} // namespace OPT

bool initParam(int argc, const char* argv[])
{
	// group of options allowed only on command line
	boost::program_options::options_description generic("Generic options");
	generic.add_options()
		("help,h", "produce this help message")
		("input-mesh,i", boost::program_options::value<std::string>(&OPT::strInputMeshFileName), "input mesh file")
		("Multiple,m", boost::program_options::value<string>(&OPT::Multiple)->default_value("5.0"), "decimation multiple (default 5.0)")
		("output-file-name,o", boost::program_options::value<std::string>(&OPT::strOutputFileName)->default_value("test_out.ply"), "output base file name")
		("output-directory,p",boost::program_options::value<std::string>(&OPT::strOutputDirName),"output directory");

	boost::program_options::options_description cmdline_options;
	cmdline_options.add(generic);

	boost::program_options::positional_options_description p;
	p.add("input-files", -1);

	try {
		// parse command line options
		boost::program_options::store(boost::program_options::command_line_parser((int)argc, argv).options(cmdline_options).positional(p).run(), OPT::vm);
		boost::program_options::notify(OPT::vm);
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
		return false;
	}

	if (OPT::vm.count("help") || OPT::strInputMeshFileName.empty()) {
		boost::program_options::options_description visible("Available options");
		visible.add(generic);
		std::cout << visible;
	}
	if (OPT::strInputMeshFileName.empty())
		return false;
	return true;
}

using namespace std;
vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1]; //不要忘了  
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}

	return res;
}