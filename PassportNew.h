//PassportNew.cpp
struct sPassport
{
	// General data
	long			m_id;
	unsigned long		m_passportLength;
	unsigned long		m_edition;
	unsigned long		m_checkSum;
	char 			m_creationgDate[12];
	char			m_nomenclature[32];
	unsigned long		m_scale;
	char			m_sheetName[32];
	// Info flags
	unsigned short 		m_flag_status 			:2;		// Data status flag
	unsigned short 		m_flag_projection 		:1;		// Projection Match flag
	unsigned short 		m_flag_realCoordinate		:2;		// Real coordinates flag
	unsigned short 		m_flag_codingType 		:2;		// Type of coding flag
	unsigned short 		m_flag_generalTable 		:1;		// Generalization table flag
	unsigned short 		m_flag_signature 		:2;		// Signature encoding flag
	unsigned short 		m_flag_precision 		:1;		// Coordinates precision flag
	unsigned short 		m_flag_specSort 		:1;		// Special data sorting flag
	unsigned short 		m_flag_reserve			:7;
	long			m_classifier;
	// Rectangular coordinates of sheet corners
	double			m_XSouthWestCorner;
	double			m_YSouthWestCorner;
	double			m_XNorthWestCorner;
	double			m_YNorthWestCorner;
	double			m_XNorthEastCorner;
	double			m_YNorthEastCorner;
	double			m_XSouthEastCorner;
	double			m_YSouthEastCorner;
	// Geodetic coordinates of sheet corners
	double			m_BSouthWestCorner;
	double			m_LSouthWestCorner;
	double			m_BNorthWestCorner;
	double			m_LNorthWestCorner;
	double			m_BNorthEastCorner;
	double			m_LNorthEastCorner;
	double			m_BSouthEastCorner;
	double			m_LSouthEastCorner;
	// Mathematical basis of sheet
	bool		 	m_ellipsoidType;
	bool			m_elevationSystem;
	bool 			m_materialProjection;
	bool			m_coordinateSystem;
	bool			m_planeUnit;				// Measure unit of plane
	bool			m_heightUnit;				// Measure unit of elevation
	bool			m_frameType;
	bool			m_mapType;
	// Reference data on source material
	char			m_updateDate[12];
	bool			m_materialKind;
	bool			m_materialType;
	char			m_reserveRef1[2];
	double			m_magneticDeclination;
	double			m_avgMeridianConv;				// Average meridians conversation
	long			m_magneticChange;				// Magnetic change per year
	char			m_declinationUpdt[12];			// Declination update date
	char			m_reserveRef2[4];
	double	 		m_reliefSection;	
	char			m_reserve[8];
	long			m_deviceResolution;
	// Location of the frame on the device
	long			m_X_SW_FrameLoacation;			// X South-West frame location
	long			m_Y_SW_FrameLoacation;			// Y South-West frame location
	long			m_X_NW_FrameLoacation;			// X North-West frame location
	long			m_Y_NW_FrameLoacation;			// Y North-West frame location
	long			m_X_NE_FrameLoacation;			// X North-East frame location
	long			m_Y_NE_FrameLoacation;			// X North-East frame location
	long			m_X_SE_FrameLoacation;			// X South-East frame location
	long			m_Y_SE_FrameLoacation;			// X South-East frame location
	long 			m_classifierCode;
	// Reference data on the projection of the source material	
	double			m_firstMainParallel;
	double			m_secondMainParallel;
	double			m_axialMeridian;
	double			m_mainPointParallel;
	double			m_poleLatitude;
	double 			m_poleLongitude;
};
