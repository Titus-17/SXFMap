//Test.cpp
#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////
template<class XY>
struct s2dPoint
{
	XY X;
	XY Y;
};
///////////////////////////////////////////////////////////////////
template <class XY, class H_>
struct s3dPoint : s2dPoint<XY>
{
	H_ H;
	//
};

// Метрики:
template<class XY>
struct sМetric_2d
{
	unsigned short headerNum;	// Номер заголовка к которому относятся метрика
	s2dPoint<XY>* p2d_points;	// Массив указателей на метрику объекта
};

template<class XY, class H>
struct sMetric_3d
{
	unsigned short headerNum;	// Номер заголовка к которому относятся метрика
	s3dPoint<XY,H> p3d_points;	// Массив указателей на метрику подобъекта
};





// Подобъекты:
template<class XY>
struct s2dSubObj
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s2dPoint<XY>* p2d_points;				// Массив указателей на метрику подобъекта
};

template<class XY, class H>
struct s3dSubObj
{
	short* pointsCount;			// Массив с количеством точек для каждого подобъекта
	short recordNum;			// Номер объекта к которому относится подобъект
	s3dPoint<XY,H> p3d_points;	// Массив указателей на метрику подобъекта
};





template<class T, template<class> class С = s2dSubObj>
struct s2PointSO
{
	С<T> A;
};

template<class T1, class T2, template<class, class> class С = s3dSubObj>
struct s3PointSO
{
	С<T1,T2> A;
};

int main()
{
	// s2PointSO<float> B;
	// B.A.p2d_points = new s2dPoint<float>;
	// B.A.p2d_points->X = 10;
	// cout << B.A.p2d_points->X << "\n";
	sМetric_2d<float> A;

	A.p2d_points = new s2dPoint<float>;
}    