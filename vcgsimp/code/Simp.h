#pragma once
/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.																											 *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.																											 *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
#include <vcg/container/simple_temporary_data.h>
#include <vcg/complex/algorithms/local_optimization.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
#include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric_tex.h>
#include "MeshIO.h"
typedef MeshIO::RTMesh CMeshO;
typedef MeshIO::RTVertex CVertexO;
namespace vcg {
	namespace tri {

		typedef	SimpleTempData<CMeshO::VertContainer, math::Quadric<double> > QuadricTemp;


		class QHelper
		{
		public:
			QHelper() {}
			static void Init() {}
			static math::Quadric<double> &Qd(CVertexO &v) { return TD()[v]; }
			static math::Quadric<double> &Qd(CVertexO *v) { return TD()[*v]; }
			static CVertexO::ScalarType W(CVertexO * /*v*/) { return 1.0; }
			static CVertexO::ScalarType W(CVertexO & /*v*/) { return 1.0; }
			static void Merge(CVertexO & /*v_dest*/, CVertexO const & /*v_del*/) {}
			static QuadricTemp* &TDp() { static QuadricTemp *td; return td; }
			static QuadricTemp &TD() { return *TDp(); }
		};

		typedef BasicVertexPair<CVertexO> VertexPair;

		class MyTriEdgeCollapse : public vcg::tri::TriEdgeCollapseQuadric< CMeshO, VertexPair, MyTriEdgeCollapse, QHelper > {
		public:
			typedef  vcg::tri::TriEdgeCollapseQuadric< CMeshO, VertexPair, MyTriEdgeCollapse, QHelper> TECQ;
			inline MyTriEdgeCollapse(const VertexPair &p, int i, BaseParameterClass *pp) :TECQ(p, i, pp) {}
		};

		class MyTriEdgeCollapseQTex : public TriEdgeCollapseQuadricTex< CMeshO, VertexPair, MyTriEdgeCollapseQTex, QuadricTexHelper<CMeshO> > {
		public:
			typedef  TriEdgeCollapseQuadricTex< CMeshO, VertexPair, MyTriEdgeCollapseQTex, QuadricTexHelper<CMeshO> > TECQ;
			inline MyTriEdgeCollapseQTex(const VertexPair &p, int i, BaseParameterClass *pp) :TECQ(p, i, pp) {}
		};

	} // end namespace tri
} // end namespace vcg

using namespace vcg;
void QuadricSimplification(CMeshO &m, int  TargetFaceNum, tri::TriEdgeCollapseQuadricParameter &pp)
{
	math::Quadric<double> QZero;
	QZero.SetZero();
	tri::QuadricTemp TD(m.vert, QZero);
	tri::QHelper::TDp() = &TD;

	if (pp.PreserveBoundary)
	{
		pp.FastPreserveBoundary = true;
		pp.PreserveBoundary = false;
	}

	if (pp.NormalCheck) pp.NormalThrRad = M_PI / 4.0;

	vcg::LocalOptimization<CMeshO> DeciSession(m, &pp);

	cout << "Initializing simplification" << std::endl;
	//cb(1, "Initializing simplification");
	DeciSession.Init<tri::MyTriEdgeCollapse >();

	DeciSession.SetTargetSimplices(TargetFaceNum);
	DeciSession.SetTimeBudget(0.1f); // this allows updating the progress bar 10 time for sec...
									 //  if(TargetError< numeric_limits<double>::max() ) DeciSession.SetTargetMetric(TargetError);
									 //int startFn=m.fn;
	int faceToDel = m.fn - TargetFaceNum;
	while (DeciSession.DoOptimization() && m.fn>TargetFaceNum)
	{
		cout << "\rSimplifying... "<< 100 - 100 * (m.fn - TargetFaceNum) / (faceToDel);
		cout << "[";
		for (int i = 0; i <= (100 - 100 * (m.fn - TargetFaceNum) / (faceToDel)) / 10; ++i)
		{
			cout << "=";
		}
		cout << "]";
	};

	DeciSession.Finalize<tri::MyTriEdgeCollapse >();

	tri::QHelper::TDp() = nullptr;
}



void QuadricTexSimplification(CMeshO &m, int  TargetFaceNum, tri::TriEdgeCollapseQuadricTexParameter &pp)
{
	tri::UpdateNormal<CMeshO>::PerFace(m);
	math::Quadric<double> QZero;
	QZero.SetZero();
	tri::QuadricTexHelper<CMeshO>::QuadricTemp TD3(m.vert, QZero);
	tri::QuadricTexHelper<CMeshO>::TDp3() = &TD3;

	std::vector<std::pair<vcg::TexCoord2<float>, Quadric5<double> > > qv;

	tri::QuadricTexHelper<CMeshO>::Quadric5Temp TD(m.vert, qv);
	tri::QuadricTexHelper<CMeshO>::TDp() = &TD;

	vcg::LocalOptimization<CMeshO> DeciSession(m, &pp);

	cout << "Initializing simplification" << std::endl;

	DeciSession.Init<tri::MyTriEdgeCollapseQTex>();

	DeciSession.SetTargetSimplices(TargetFaceNum);
	DeciSession.SetTimeBudget(0.1f);
	//	int startFn=m.fn;

	int faceToDel = m.fn - TargetFaceNum;

	while (DeciSession.DoOptimization() && m.fn>TargetFaceNum)
	{
		cout << "\rSimplifying... " << 100 - 100 * (m.fn - TargetFaceNum) / (faceToDel);
		cout << "[";
		for (int i = 0; i <= (100 - 100 * (m.fn - TargetFaceNum) / (faceToDel)) / 10; ++i)
		{
			cout << "=";
		}
		cout << "]";

	};

	DeciSession.Finalize<tri::MyTriEdgeCollapseQTex>();

	tri::QuadricTexHelper<CMeshO>::TDp3() = nullptr;
	tri::QuadricTexHelper<CMeshO>::TDp() = nullptr;

}