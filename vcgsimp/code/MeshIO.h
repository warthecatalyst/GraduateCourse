#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NDEBUG
#include <vector>
#include <limits>
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/complex/algorithms/refine.h>
#include <vcg/complex/algorithms/refine_loop.h>


#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/stat.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/smooth.h>
#include <vcg/complex/algorithms/hole.h>
#include <vcg/complex/algorithms/polygon_support.h>
#include <vcg/complex/algorithms/update/position.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/update/selection.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>

#include <vcg/complex/complex.h>
#include <vcg/simplex/face/component_ep.h>
#include<vcg/complex/algorithms/update/normal.h>
#include<vcg/complex//algorithms/smooth.h>
//#include <wrap/gl/trimesh.h>
// input output
#include <wrap/ply/plylib.h>
#include <wrap/io_trimesh/import_ply.h>
#include <wrap/io_trimesh/import_obj.h>

#include <wrap/io_trimesh/export_ply.h>
#include <wrap/io_trimesh/export_obj.h>
#include <wrap/io_trimesh/import_fbx.h>
namespace MeshIO {
	class RTVertex; class RTEdge; class RTFace;
	struct RTUsedTypes : public vcg::UsedTypes<
		vcg::Use<RTVertex>::AsVertexType,
		vcg::Use<RTEdge>  ::AsEdgeType,
		vcg::Use<RTFace>  ::AsFaceType   > {};

	class RTVertex : public vcg::Vertex<RTUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::VFAdj, vcg::vertex::Mark, vcg::vertex::BitFlags, vcg::vertex::Qualityf,vcg::vertex::TexCoord2f> {};
	class RTFace : public vcg::Face<  RTUsedTypes, vcg::face::VertexRef, vcg::face::Normal3f, vcg::face::FFAdj, vcg::face::VFAdj, vcg::face::Mark, vcg::face::BitFlags, vcg::face::Color4b,vcg::face::WedgeTexCoord2f/*, vcg::face::WedgeTexCoordfOcf*/> {};
	class RTEdge : public vcg::Edge<  RTUsedTypes, vcg::edge::VertexRef, vcg::edge::VEAdj, vcg::edge::EFAdj, vcg::edge::EEAdj, vcg::edge::BitFlags> {};

	class RTMesh : public vcg::tri::TriMesh< std::vector<RTVertex>, std::vector<RTFace>, std::vector<RTEdge> > {};

	typedef RTMesh MyMesh;
	typedef MyMesh::ScalarType ScalarType;
	typedef MyMesh::CoordType CoordType;
	typedef MyMesh::FaceType FaceType;
	typedef MyMesh::FaceIterator Fi;
	typedef MyMesh::VertexIterator Vi;
	typedef MyMesh::VertexType VertexType;
}

class VCGTriangleMesh
{
public:
	VCGTriangleMesh(void) {};
	~VCGTriangleMesh(void) {};

public:
	//读入文件
	void read_file(std::string filename);
	//将文件保存到文件
	void save_file(std::string pathOut, std::string filename);


	MeshIO::MyMesh m_mesh;
	//MeshIO::MyMesh m_obj;

};