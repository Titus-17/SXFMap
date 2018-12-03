//PassportNew.h
struct sPassport
{
	// General data
	long			id;
	unsigned long		passportLength;
	unsigned long		edition;
	unsigned long		checkSum;
	char 			creationgDate[12];
	char			nomenclature[32];
	unsigned long		scale;
	char			sheetName[32];
	// Info flags
	unsigned short 		flag_status 			:2;		// Data status flag
	unsigned short 		flag_projection 		:1;		// Projection Match flag
	unsigned short 		flag_realCoordinate		:2;		// Real coordinates flag
	unsigned short 		flag_codingType 		:2;		// Type of coding flag
	unsigned short 		flag_generalTable 		:1;		// Generalization table flag
	unsigned short 		flag_signature 			:2;		// Signature encoding flag
	unsigned short 		flag_precision 			:1;		// Coordinates precision flag
	unsigned short 		flag_specSort 			:1;		// Special data sorting flag
	unsigned short 		flag_reserve			:7;
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
	// Geodetic coordinates of sheet corners
	double			BSouthWestCorner;
	double			LSouthWestCorner;
	double			BNorthWestCorner;
	double			LNorthWestCorner;
	double			BNorthEastCorner;
	double			LNorthEastCorner;
	double			BSouthEastCorner;
	double			LSouthEastCorner;
	// Mathematical basis of sheet
	bool		 	ellipsoidType;
	bool			elevationSystem;
	bool 			materialProjection;
	bool			coordinateSystem;
	bool			planeUnit;				// Measure unit of plane
	bool			heightUnit;				// Measure unit of elevation
	bool			frameType;
	bool			mapType;
	// Reference data on source material
	char			updateDate[12];
	bool			materialKind;
	bool			materialType;
	char			reserveRef1[2];
	double			magneticDeclination;
	double			avgMeridianConv;				// Average meridians conversation
	long			magneticChange;				// Magnetic change per year
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
