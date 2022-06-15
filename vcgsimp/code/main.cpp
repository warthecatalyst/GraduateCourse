#include "MeshIO.h"
#include "ArgsParse.h"
#include "Simp.h"
#include "FileParse.h"


int main(int argc, const char * argv[])
{
	if (!initParam(argc,argv))
	{
		std::cout << "Parameter error\n";
		return EXIT_FAILURE;
	}

	const string  fileName = OPT::strInputMeshFileName;
	string Multiple = OPT::Multiple;
	vector<string> multiplestrings = split(Multiple, ",");
	double maxDecimationError = 1.0;
	const string  pathOut = OPT::strOutputDirName.empty()? fileName.substr(0, fileName.find_last_of("\\")):OPT::strOutputDirName;
	const string  ofileName = OPT::strOutputFileName;

	cout << "+ Parameters" << std::endl;
	cout << "\t input                      " << fileName << std::endl;
	cout << "\t max. decimation error      " << maxDecimationError << std::endl;
	cout << "\t output path                " << pathOut << std::endl;
	cout << "\t output file                " << ofileName << std::endl;
	cout << "\t multiple num               " << Multiple << std::endl;
	cout << "+ Load mesh " << endl;

	VCGTriangleMesh* m_vcg_mesh = new VCGTriangleMesh;

	//从此处开始计时
	auto starttime = clock();
	m_vcg_mesh->read_file(fileName);

	int vnum = m_vcg_mesh->m_mesh.vn;
	int fnum = m_vcg_mesh->m_mesh.fn;

	vector<size_t> targetNTrianglesDecimatedMesh;
	vector<size_t> targetNVerticesDecimatedMesh;
	for (int i = 0; i < multiplestrings.size(); i++)
	{
		float decimationRatio = atof(multiplestrings[i].c_str());
		targetNTrianglesDecimatedMesh.push_back(size_t(fnum/ decimationRatio));
		targetNVerticesDecimatedMesh.push_back(size_t(vnum / decimationRatio));
	}

	vcg::tri::TriEdgeCollapseQuadricParameter para;
	para.QualityThr = 0.3; // Quality Threshold for penalizing bad shaped faces: the value is in the range [0..1], 0 accept any kind of face (no penalties), 0.5 penalize faces with quality < 0.5, proportionally to their shape
	//para.PreserveBoundary = true; // the simplification process tries to not affect mesh boundaries during simplification, used for test
	para.PreserveBoundary = false; // the simplification process tries to not affect mesh boundaries during simplification
	para.QualityWeight = false; // use the Per-Vertex quality as a weighting factor for the simplification: the weight is used as an error amplification value, so a vertex with a high quality value will not be simplified and a portion of the mesh with low quality values will be aggressively simplified
	para.NormalCheck = false; // try to avoid face flipping effects and try to preserve the original orientation of the surface
	para.OptimalPlacement = true; // each collapsed vertex is placed in the position minimizing the quadric error; it can fail (creating bad spikes) in case of very flat areas; if disabled edges are collapsed onto one of the two original vertices and the final mesh is composed by a subset of the original vertices
	para.QualityQuadric = false; // add additional simplification constraints that improves the quality of the simplification of the planar portion of the mesh

	vcg::tri::TriEdgeCollapseQuadricTexParameter para1;
	para1.QualityThr = 0.3;
	para1.PreserveBoundary = false;
	para1.ExtraTCoordWeight = 1.0;
	para1.OptimalPlacement = true;
	para1.QualityQuadric = false;
	para1.NormalCheck = false;
	
	cout << sizeof(string) << endl;
	cout << "Before Simplification: \t\n";
	cout << "the number of faces: " << m_vcg_mesh->m_mesh.fn << endl;	//output the number of faces before Simplification
	cout << "the number of vertexes: " << m_vcg_mesh->m_mesh.vn << endl; //output the number of vertexes before Simplification

	//QuadricSimplification(m_vcg_mesh->m_mesh, targetNTrianglesDecimatedMesh[0], para);
	QuadricTexSimplification(m_vcg_mesh->m_mesh, targetNTrianglesDecimatedMesh[0], para1);
	cout << "\r Simplification Done!\t\t\t\n";
	cout << "After Simplification: \t\t\n";
	cout << "the number of faces: " << m_vcg_mesh->m_mesh.fn << endl;	//output the number of faces before Simplification
	cout << "the number of vertexes: " << m_vcg_mesh->m_mesh.vn << endl; //output the number of vertexes before Simplification
	cout << "Result saving....\n";
	//m_vcg_mesh->save_file(pathOut+"\\"+ofileName);
	m_vcg_mesh->save_file(pathOut,ofileName);
	//此处结束计时
	auto endtime = clock();
	cout << "Done! Result saved!\n";
	cout << "Time elapsed:" << (double)(endtime - starttime) / CLOCKS_PER_SEC << "s" << std::endl;

}
