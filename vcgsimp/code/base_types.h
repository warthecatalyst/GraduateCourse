
#ifndef MESHLAB_BASE_TYPES_H
#define MESHLAB_BASE_TYPES_H

#include <vcg/complex/complex.h>
#define MESHLAB_SCALAR double

typedef MESHLAB_SCALAR Scalarm;
typedef vcg::Point2<MESHLAB_SCALAR>     Point2m;
typedef vcg::Point3<MESHLAB_SCALAR>     Point3m;
typedef vcg::Point4<MESHLAB_SCALAR>     Point4m;
typedef vcg::Plane3<MESHLAB_SCALAR>     Plane3m;
typedef vcg::Segment2<MESHLAB_SCALAR>   Segment2m;
typedef vcg::Segment3<MESHLAB_SCALAR>   Segment3m;
typedef vcg::Box3<MESHLAB_SCALAR>       Box3m;
typedef vcg::Matrix44<MESHLAB_SCALAR>   Matrix44m;
typedef vcg::Matrix33<MESHLAB_SCALAR>   Matrix33m;
typedef vcg::Shot<MESHLAB_SCALAR>       Shotm;
typedef vcg::Similarity<MESHLAB_SCALAR> Similaritym;

template<typename T>
struct MeshLabScalarTest
{
};

template<>
struct MeshLabScalarTest<float>
{
	static const char* floatingPointPrecision() { return "fp"; }
	static const char* floatingPointPrecisionIOToken() { return "%f"; }
	static bool doublePrecision() { return false; }
};

template<>
struct MeshLabScalarTest<double>
{
	static const char* floatingPointPrecision() { return "dp"; }
	static const char* floatingPointPrecisionIOToken() { return "%lf"; }
	static bool doublePrecision() { return true; }
};

#endif // MESHLAB_BASE_TYPES_H
