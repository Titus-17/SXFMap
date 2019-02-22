//PassportNew.h

#ifndef STRUCTS_H
#define STRUCTS_H

//=============================Паспорт=============================
#pragma pack (push, 1)
struct sPassport	// 400 Bytes
{
	// Общие данные
	long			id;								// Идентификатор файла
	unsigned long	length;							// Длина паспорта в байтах
	unsigned long	edition;						// Редакция формата
	unsigned long	checkSum;						// Контрольная сумма
	char 			creationgDate[12];				// Дата создания набора данных
	char			nomenclature[32];				// Номенклатура листа
	unsigned long	scale;							// Масштаб
	char			sheetName[32];					// Условное название листа

	// Информационные флаги
	unsigned char 	flag_status 				:2;	// Флаг состояния данных
	unsigned char 	flag_projection 			:1;	// Флаг соответствия проекции
	unsigned char 	flag_realCoordinate			:2;	// Флаг наличия реальных координат                
	unsigned char 	flag_codingType 			:2;	// Флаг способа кодирования
	unsigned char 	flag_generalTable 			:1;	// Таблица генерализации    
	unsigned char 	flag_signature;					// Флаг кодировки подписи 
	unsigned char 	flag_precision;					// Флаг точности координат
	unsigned char 	flag_specSort 				:1;	// Признак специальной сортировки данных
	unsigned char 	flag_reserve				:7;	// Резерв
	long			classifier;

	// Прямоугольные координаты углов листа
	double			XSouthWestCorner;
	double			YSouthWestCorner;
	double			XNorthWestCorner;
	double			YNorthWestCorner;
	double			XNorthEastCorner;
	double			YNorthEastCorner;
	double			XSouthEastCorner;
	double			YSouthEastCorner;

	// Геодезические координаты углов листа:            
	double			BSouthWestCorner;
	double			LSouthWestCorner;
	double			BNorthWestCorner;
	double			LNorthWestCorner;
	double			BNorthEastCorner;
	double			LNorthEastCorner;
	double			BSouthEastCorner;
	double			LSouthEastCorner;

	// Математическая основа листа
	unsigned char	ellipsoidType;					// Вид эллипсоида
	unsigned char	elevationSystem;				// Система высот
	unsigned char 	materialProjection;				// Проекция исходного мат. 
	unsigned char	coordinateSystem;				// Система координат 
	unsigned char	planeUnit;						// Единица измерения в плане
	unsigned char	heightUnit;						// Единица измерения по высоте                  
	unsigned char	frameType;						// Вид рамки
	unsigned char	mapType;						// Обобщенный тип карты 

	// Справочные данные по исходному материалу
	char			updateDate[12];					// Дaтa съемки местности или обновления карты    

	unsigned char	materialKind;					// Вид исходного материала
	unsigned char	materialType;					// Тип исходного матеpиaлa
	char			reserveRef1[2];					// Резерв
	double			magneticDeclination;			// Мaгнитное склонение
	double			avgMeridianConv;				// Сpеднее сближение меpидиaнов               
	double			magneticChange;					// Годовое изменение магнитного склонения     
	char			declinationUpdt[12];			// Дата измерения склонения
	char			reserveRef2[4];					// Резеpв 
	double	 		reliefSection;					// Высотa сечения pельефa 
	char			reserve[8];						// Резерв

	long			deviceResolution;				// Разрешающая способность прибора

	// Расположение рамки на приборе
	long			X_SW_FrameLoacation;			//
	long			Y_SW_FrameLoacation;			//
	long			X_NW_FrameLoacation;			//
	long			Y_NW_FrameLoacation;			//
	long			X_NE_FrameLoacation;			//
	long			Y_NE_FrameLoacation;			//
	long			X_SE_FrameLoacation;			//
	long			Y_SE_FrameLoacation;			//

	long 			classifierCode; 				// Классификационный код рамки объекта              

	// Справочные данные по проекции исходного материала
	double			firstMainParallel;				// Пеpвaя глaвнaя пapaллель 
	double			secondMainParallel;				// Втоpaя глaвнaя пapaллель    
	double			axialMeridian;					// Осевой меpидиaн
	double			mainPointParallel;				// Пapaллель главной точки
	double			poleLatitude;					// Широта полюса
	double 			poleLongitude;					// Долгота полюса
};
#pragma pack (pop)
//=================================================================


//=============================Дескриптор==========================
#pragma pack (push, 1)
struct sDescriptor	// 52 Bytes
{	
	long 			id;								// Идентификатор начала записи
	unsigned long	length;							// Длина дескриптора = 52
	char			nomenclature[32];				// Номенклатура листа
	long 			recordCount;					// Число записей данных
	// Info flags
	unsigned char 	flag_status 				:2;	// Флаг состояния данных
	unsigned char 	flag_projection 			:1;	// Флаг соответствия проекции
	unsigned char 	flag_realCoordinate			:2;	// Флаг наличия реальных координат
	unsigned char 	flag_codingType 			:2;	// Флаг способа кодирования
	unsigned char 	flag_generalTable 			:1;	// Таблица генерализации

	unsigned char	flag_signature;					// Флаг кодирования подписей
	unsigned short 	flag_reserve;					// Резерв

	unsigned short	objectsClassifier;				// Классификатор объектов
	unsigned short	semanticsClassifier;			// Классификатор семантики
};
#pragma pack (pop)
//=================================================================


//==========================Заголовок записи=======================
#pragma pack (push, 1)
struct sHeader		// 32 Bytes
{
	long			id;								// Идентификатор начала записи
	unsigned long	length;							// Общая длина записи с заголовком
	unsigned long 	metricLenght;					// Длина метрики
	long 			classificationCode;				// Классификационный код

	unsigned short	numberInGroup;					// Номер в группе
	unsigned short	groupNumber;					// Номер группы

	unsigned short	flag_localization			:4;	// Характер локализации
	unsigned short	flag_frameExit				:4; // Выход за рамки
	unsigned short	flag_insularity				:1;	// Признак замкнутости
	unsigned short	flag_semantics				:1;	// Наличие семантики
	unsigned short	flag_metricSize				:1; // Размер элемента метрики
	unsigned short	flag_bindingVector			:1; // Наличие вектора привязки
	unsigned short	flag_UNICODE				:1; // Признак текста в UNICODE
	unsigned short	flag_reserve				:3; // Резерв
	
	unsigned short	flag_metricFormat			:1; // Формат записи метрики
	unsigned short	flag_dimension				:1; // Размерность представления
	unsigned short	flag_metricType				:1; // Тип элемента метрики
	unsigned short	flag_metricText				:1; // Признак метрики с текстом
	unsigned short	flag_sign					:1; // Наличие графики (знака)
	unsigned short	flag_scalableGraphics		:1; // Масштабируемость графики
	unsigned short	flag_metricSpline			:2; // Признак построения сплайна по метрике

	unsigned short	bottomBorder				:4;	// Нижняя граница видимости
	unsigned short	topBorder 					:4;	// Верхняя граница видимости

	unsigned long 	metricPointsCountBig;			// Число точек метрики для больших объектов
	unsigned short 	subobjectCount;					// Число подобъектов 
	unsigned short 	metricPointsCount;				// Число точек метрики
};
#pragma pack (pop)
//=================================================================




//=========================Точки 2D/3D=============================
template<class XY>
struct s2dPoint
{
	XY X;
	XY Y;
};

template <class XY, class H_>
struct s3dPoint : s2dPoint<XY>
{
	H_ H;
	//
};
//================================================================



//=========================Метрики 2D/3D==========================
template<class XY>
struct sМetric_2d
{
	sHeader* header;			// Указатель на заголовок
	s2dPoint<XY>* p2d_points;	// Массив указателей на метрику объекта
};

template<class XY, class H>
struct sMetric_3d
{
	sHeader* header;
	s3dPoint<XY,H>* p3d_points;	// Массив указателей на метрику подобъекта
};
//================================================================



//===================Метрики подобъектов 2D/3D====================
template<class XY>
struct sМetricSO_2d
{
	sHeader* header;			// Указатель на заголовок
	// Массив указателей на количество точек в каждом подобъекте:
	unsigned short** pointsCount;
	s2dPoint<XY>** p2d_points;	// Массив указателей на метрики подобъектов
};

template<class XY, class H>
struct sMetricSO_3d
{
	sHeader* header;				// Указатель на заголовок
	// Массив указателей на место в памяти с количеством точек для каждого подобъекта:
	unsigned short** pointsCount;	
	s3dPoint<XY,H>** p3d_points;	// Массив указателей на метрики подобъектов
};
//================================================================



struct s2dShortSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	// Массив указателей на место в памяти с количеством точек для каждого подобъекта:
	unsigned short** pointsCount;			
	s2dPoint<short>* p2d_points;			// Массив указателей на метрику подобъекта
};

struct s2dLongSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s2dPoint<long>* p2d_points;				// Массив указателей на метрику подобъекта
};

struct s2dFloatSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s2dPoint<float>* p2d_points;			// Массив указателей на метрику подобъекта
};

struct s2dDoubleSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s2dPoint<double>* p2d_points;				// Массив указателей на метрику подобъекта
};

struct s3dShortSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s3dPoint<short, float>* p3d_points;				// Массив указателей на метрику подобъекта
};

struct s3dLongSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s3dPoint<long, float>* p3d_points;				// Массив указателей на метрику подобъекта
};

struct s3dFloatSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s3dPoint<float, float>* p3d_points;				// Массив указателей на метрику подобъекта
};

struct s3dDoubleSO
{
	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
	s3dPoint<double, double>* p3d_points;				// Массив указателей на метрику подобъекта
};

// template<template<class> class T>
// template<class T, template<class> class С = s2dSubObj>
// struct s2PointSO
// {
// 	С<T> p2d_points;
// };


// template<class XY>
// struct s2dSubObj
// {
// 	unsigned short recordNum;				// Номер объекта к которому относятся подобъекты
// 	unsigned short** pointsCount;			// Массив с количеством точек для каждого подобъекта
// 	s2dPoint<XY>* p2d_points;				// Массив указателей на метрику подобъекта
// };

// template<class XY, class H>
// struct s3dSubObj
// {
// 	short* pointsCount;			// Массив с количеством точек для каждого подобъекта
// 	short recordNum;			// Номер объекта к которому относится подобъект
// 	s3dPoint<XY,H> p3d_points;	// Массив указателей на метрику подобъекта
// };


#endif