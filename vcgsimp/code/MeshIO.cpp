#include "MeshIO.h"
#include <Windows.h>
using namespace MeshIO;

void VCGTriangleMesh::read_file(std::string filename)
{
	//改成能读.obj格式的数据
	auto index = filename.find_last_of('.');
	if (index == string::npos) {
		cout << "wrong type of input file" << endl;
		exit(0);
	}
	string extension = filename.substr(index+1);
	cout << "input extension = " << extension << endl;
	std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
	if (extension == "obj") {
		int mask;
		int err = vcg::tri::io::ImporterOBJ<MyMesh>::Open(m_mesh, filename.c_str(),mask);
		if (err != 0) {
			const char* errmsg = vcg::tri::io::ImporterOBJ<MyMesh>::ErrorMsg(err);
			cout << errmsg << " " << filename << endl;
			if (err >= 6) {
				cout << "fatal error!!" << endl;
				exit(0);
			}
		}
	}
	else if (extension == "ply") {
		int err = vcg::tri::io::ImporterPLY<MyMesh>::Open(m_mesh, filename.c_str());
		if (err != 0) {
			const char* errmsg = vcg::tri::io::ImporterPLY<MyMesh>::ErrorMsg(err);
			cout << errmsg << filename;
			exit(0);
		}
	}
	else if (extension == "fbx") {	//输入extension为FBX完成
		int err = vcg::tri::io::ImporterFBX<MyMesh>::Open(m_mesh, filename.c_str());
		if (err != 0) {
			auto errmsg = vcg::tri::io::ImporterFBX<MyMesh>::ErrorMsg(err);
			cout << errmsg << filename;
			exit(0);
		}
	}
	else {
		cout << "wrong input extension type" << endl;
		exit(0);
	}
}

void VCGTriangleMesh::save_file(std::string pathOut,std::string filename)
{
	//改成能输出.obj格式的数据
	//cout <<"input desdir = " << pathOut << endl;
	wchar_t curdir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, curdir);
	int len = MultiByteToWideChar(CP_ACP, 0, pathOut.c_str(), pathOut.size(), NULL, 0);
	TCHAR* desdir = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0,pathOut.c_str(), pathOut.size(), desdir, len);
	desdir[len] = '\0';             //添加字符串结尾
	//wcout << "desdir = " << desdir << endl;
	SetCurrentDirectory(desdir);
	//wchar_t curdir1[MAX_PATH] = { 0 };
	//GetCurrentDirectory(MAX_PATH, curdir1);
	//wcout << "curdir1 = " << curdir1 << endl;
	auto index = filename.find_last_of('.');
	if (index == string::npos) {
		cout << "wrong type of output file" << endl;
		exit(0);
	}
	string extension = filename.substr(index + 1);
	cout << "output extension = " << extension << endl;
	std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
	if (extension == "fbx") {
		filename = filename.substr(0, index) + ".obj";
		//std::cout << "after transfer filename = " << filename << std::endl; // for debug
	}
	if (extension == "obj"||extension == "fbx") {	
		int err = vcg::tri::io::ExporterOBJ<MyMesh>::Save(m_mesh, filename.c_str(), vcg::tri::io::Mask::IOM_BITPOLYGONAL| vcg::tri::io::ExporterOBJ<MyMesh>::GetExportMaskCapability());
		if (err != 0) {
			const char* errmsg = vcg::tri::io::ExporterOBJ<MyMesh>::ErrorMsg(err);
			cout << errmsg << filename;
			exit(0);
		}
	}
	else if (extension == "ply") {
		int err = vcg::tri::io::ExporterPLY<MyMesh>::Save(m_mesh, filename.c_str(), vcg::tri::io::Mask::IOM_FACECOLOR + vcg::tri::io::Mask::IOM_VERTCOLOR);
		if (err != 0) {
			const char* errmsg = vcg::tri::io::ExporterPLY<MyMesh>::ErrorMsg(err);
			cout << errmsg << filename;
			exit(0);
		}
	}
	else {
		cout << "wrong output extension type" << endl;
		exit(0);
	}
	SetCurrentDirectory(curdir);
}