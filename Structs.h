//PassportNew.h

#ifndef STRUCTS_H
#define STRUCTS_H

////////////////////////////////////////////////////////////////////
#pragma pack (push, 1)
struct sPassport	// 400 Bytes
{
	// General data
	long			id;
	unsigned long	length;
	unsigned long	edition;
	unsigned long	checkSum;
	char 			creationgDate[12];
	char			nomenclature[32];
	unsigned long	scale;
	char			sheetName[32];

	// Info flags
	unsigned char 	flag_status 			:2;		// Data status flag
	unsigned char 	flag_projection 		:1;		// Projection Match flag
	unsigned char 	flag_realCoordinate		:2;		// Real coordinates flag
	unsigned char 	flag_codingType 		:2;		// Type of coding flag
	unsigned char 	flag_generalTable 		:1;		// Generalization table flag
	unsigned char 	flag_signature;					// Signature encoding flag
	unsigned char 	flag_precision;					// Coordinates precision flag
	unsigned char 	flag_specSort 			:1;		// Special data sorting flag
	unsigned char 	flag_reserve			:7;
	long			classifier;

	// Rectangular coordinates of sheet corners
	double			XSouthWestCorner;
	double			YSouthWestCorner;
	double			XNorthWestCorner;
	double			YNorthWestCorner;
	double			XNorthEastCorner;
	double			YNorthEastCorner;
	double			XSouthEastCorner;
	double			YSouthEastCorner;

	// // Geodetic coordinates of sheet corners
	double			BSouthWestCorner;
	double			LSouthWestCorner;
	double			BNorthWestCorner;
	double			LNorthWestCorner;
	double			BNorthEastCorner;
	double			LNorthEastCorner;
	double			BSouthEastCorner;
	double			LSouthEastCorner;

	// Mathematical basis of sheet
	unsigned char	ellipsoidType;
	unsigned char	elevationSystem;
	unsigned char 	materialProjection;
	unsigned char	coordinateSystem;
	unsigned char	planeUnit;						// Measure unit of plane
	unsigned char	heightUnit;						// Measure unit of elevation
	unsigned char	frameType;
	unsigned char	mapType;

	// // Reference data on source material
	char			updateDate[12];
	unsigned char	materialKind;
	unsigned char	materialType;
	char			reserveRef1[2];
	double			magneticDeclination;
	double			avgMeridianConv;				// Average meridians conversation
	double			magneticChange;					// Magnetic change per year
	char			declinationUpdt[12];			// Declination update date
	char			reserveRef2[4];
	double	 		reliefSection;	
	char			reserve[8];

	long			deviceResolution;

	// Location of the frame on the device
	long			X_SW_FrameLoacation;			// X South-West frame location
	long			Y_SW_FrameLoacation;			// Y South-West frame location
	long			X_NW_FrameLoacation;			// X North-West frame location
	long			Y_NW_FrameLoacation;			// Y North-West frame location
	long			X_NE_FrameLoacation;			// X North-East frame location
	long			Y_NE_FrameLoacation;			// X North-East frame location
	long			X_SE_FrameLoacation;			// X South-East frame location
	long			Y_SE_FrameLoacation;			// X South-East frame location
	long 			classifierCode;

	// Reference data on the projection of the source material	
	double			firstMainParallel;
	double			secondMainParallel;
	double			axialMeridian;
	double			mainPointParallel;
	double			poleLatitude;
	double 			poleLongitude;
};
#pragma pack (pop)
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
#pragma pack (push, 1)
struct sDescriptor	// 52 Bytes
{	
	long 			id;
	unsigned long	length;
	char			nomenclature[32];
	long 			recordCount;
	// Info flags
	unsigned char 	flag_status 			:2;		// Data status flag
	unsigned char 	flag_projection 		:1;		// Projection Match flag
	unsigned char 	flag_realCoordinate		:2;		// Real coordinates flag
	unsigned char 	flag_codingType 		:2;		// Type of coding flag
	unsigned char 	flag_generalTable 		:1;		// Generalization table flag

	unsigned char	flag_signature;					// Signature encoding flag
	unsigned short 	flag_reserve;

	unsigned short	objectsClassifier;	
	unsigned short	semanticsClassifier;
};
#pragma pack (pop)
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
#pragma pack (push, 1)
struct sHeader		// 32 Bytes
{
	long			id;
	unsigned long	length;
	unsigned long 	metricLenght;
	long 			classificationCode;

	unsigned short	numberInGroup;
	unsigned short	groupNumber;

	unsigned short	flag_localization			:4;
	unsigned short	flag_frameExit				:4;
	unsigned short	flag_insularity				:1;
	unsigned short	flag_semantics				:1;
	unsigned short	flag_metricSize				:1;
	unsigned short	flag_bindingVector			:1;
	unsigned short	flag_UNICODE				:1;
	unsigned short	flag_reserve				:3;
	
	unsigned short	flag_metricFormat			:1;
	unsigned short	flag_dimension				:1;
	unsigned short	flag_metricType				:1;
	unsigned short	flag_metricText				:1;
	unsigned short	flag_sign					:1;
	unsigned short	flag_scalableGraphics		:1;
	unsigned short	flag_metricSpline			:2;

	unsigned short	bottomBorder		:4;
	unsigned short	topBorder 			:4;

	unsigned long 	metricPointsCountBig;
	unsigned short 	subobjectNumber;
	unsigned short 	metricPointsCount;
};
#pragma pack (pop)
///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
template<class TypeXY>
struct s2dPoint
{
	TypeXY X;
	TypeXY Y;
};
///////////////////////////////////////////////////////////////////
template <class TypeXY, class TypeH>
struct s3dPoint : s2dPoint<TypeXY>
{
	TypeH H;
	//
};



///////////////////////////////////////////////////////////////////
struct sRecord
{
	sHeader*	header;
	void*		points;
	void**		subobjectPoints;

	// void** 		subPoints; 

	bool is2dPoint()	// Определение размерности метрики (2D/3D)
	{
		if (header->flag_dimension == 0)
			return true;
		else
			return false;
	}
	bool isInteger()	// Определение типа метрики
	{
		if (header->flag_metricType == 0)
			return true;
		else
			return false;
	}
	bool isShortSize()	// Определение длины метрики
	{
		if(header->flag_metricSize == 0)
			return true;
		else
			return false;
	}
};
///////////////////////////////////////////////////////////////////


#endif