//Map.cpp
#include <iostream>
#include "Map.h"

using namespace std;

cMap::cMap()
{
	m_ptr_charMap 		= NULL;
	m_ptr_passport 		= NULL;
	m_ptr_descriptor 	= NULL;

	m_p2d_ShortCount	= 0;
	m_p2d_LongCount		= 0;
	m_p2d_FloatCount	= 0;
	m_p2d_DoubleCount	= 0;
	m_p3d_ShortCount	= 0;
	m_p3d_LongCount		= 0;
	m_p3d_FloatCount	= 0;
	m_p3d_DoubleCount	= 0;

	m_p2d_Short 		= NULL;
	m_p2d_Long 			= NULL;
	m_p2d_Float 		= NULL;
	m_p2d_Double 		= NULL;
	m_p3d_Short 		= NULL;
	m_p3d_Long 			= NULL;
	m_p3d_Float 		= NULL;
	m_p3d_Double 		= NULL;
}
/////////////////////////////////////////////////////////////////////
cMap::~cMap()
{
	// m_ptr_passport 		= NULL;
	// m_ptr_descriptor 	= NULL;
	// m_ptr_records 		= NULL;

	// m_p2d_Short 			= NULL;
	// m_p2d_Long 			= NULL;
	// m_p2d_Float 			= NULL;
	// m_p2d_Double 		= NULL;
	// m_p3d_Short 			= NULL;
	// m_p3d_Long 			= NULL;
	// m_p3d_Float 			= NULL;
	// m_p3d_Double 		= NULL;

	// if (m_ptr_charMap != NULL)
	// {
	// 	delete[] m_ptr_charMap;
	// 	cout << "Proc\n";
	// }
	// if (m_ptr_records != NULL)
	// 	delete[] m_ptr_records;

	// if (m_p2d_ShortCount > 0)
	// 	delete[] m_p2d_Short;
	// if (m_p2d_LongCount > 0)
	// 	delete[] m_p2d_Long;
	// if (m_p2d_FloatCount > 0)
	// 	delete[] m_p2d_Float;
	// if (m_p2d_DoubleCount > 0)
	// 	delete[] m_p2d_Double;
	// if (m_p3d_ShortCount > 0)
	// 	delete[] m_p3d_Short;
	// if (m_p3d_LongCount > 0)
	// 	delete[] m_p3d_Long;
	// if (m_p3d_FloatCount > 0)
	// 	delete[] m_p3d_Float;
	// if (m_p3d_DoubleCount > 0)
	// 	delete[] m_p3d_Double;

}
/////////////////////////////////////////////////////////////////////
short cMap::readSXF(char* p_filename)
{
	ifstream infile;
	infile.open(p_filename, ios::binary);				// Проверка на возможность чтения карты
	if (!infile){
		cerr << "Unable to read SXF!\n";
		infile.close();
		return (0);
	}
	
	infile.seekg(0,ios::end);							// Нахождение размера карты
	unsigned long endPosition = infile.tellg();
	infile.seekg(0,ios::beg);
	m_ptr_charMap = new char[endPosition];				// Выделение памяти под всю сырую (битовую) карту
	infile.read((char*)(m_ptr_charMap), endPosition);	// Сохранение сырой карты в массив
	infile.close();

	cout << "Map was read\n";
	return 1;
}
/////////////////////////////////////////////////////////////////////
// short cMap::getPointsSize(long i)
// {
// 	if (m_ptr_records[i].is2dPoint())				// Определение размерности метрики (2D/3D)
// 	{
// 		if (m_ptr_records[i].isInteger())			// Определение типа данных метрика (Целое/Плав.т.)
// 		{						
// 			if(m_ptr_records[i].isShortSize())		// Определение длины типа данных (2-4 байта/4-8 байта)
// 				return 2*sizeof(short);
// 			else									// long
// 				return 2*sizeof(long);
// 		}
// 		else 										// Формат плавающей точки
// 		{
// 			if(m_ptr_records[i].isShortSize())		// float
// 				return 2*sizeof(float);
// 			else 									// double
// 				return 2*sizeof(double);
// 		}
// 	}
// 	else 											// Метрика в 3D
// 	{
// 		if (m_ptr_records[i].isInteger())			// Целый тип
// 		{
// 			if(m_ptr_records[i].isShortSize())		// short/float, где float - высота
// 				return (2*sizeof(short) + sizeof(float));
// 			else 									// long/float
// 				return (2*sizeof(long) + sizeof(float));
// 		}
// 		else 										// Плавающая точка
// 		{
// 			if(m_ptr_records[i].isShortSize())		// float/float
// 				return 3*sizeof(float);
// 			else 									// double/double
// 				return 3*sizeof(double);
// 		}
// 	}
// }
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
void cMap::calcPointsCount()
{
	m_ptr_passport 		= reinterpret_cast<sPassport*>		(m_ptr_charMap);
	m_ptr_descriptor 	= reinterpret_cast<sDescriptor*>	(m_ptr_charMap + sizeof(sPassport));
	m_ptr_headers = new sHeader*[m_ptr_descriptor->recordCount];	

	unsigned long currentPosition;	// Количество байт данных карты, которые
									// уже были интерпретированы.

	// Первые 452 байта пасспорта и дескриптора уже были интерпретированы:
	currentPosition = sizeof(sPassport) + sizeof(sDescriptor);

	// Цикл по количеству записей в карте
	for(unsigned long i = 0; i < m_ptr_descriptor->recordCount; i ++)	
	{
		m_ptr_headers[i] = reinterpret_cast<sHeader*>	(m_ptr_charMap + currentPosition);
		if (m_ptr_headers[i]->flag_dimension == 0)				// Определение размерности метрики (2D/3D)
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Определение типа данных метрика (Целое/Плав.т.)
			{						
				if(m_ptr_headers[i]->flag_metricSize == 0)		// Определение длины типа данных (2-4 B/4-8 B)
					m_p2d_ShortCount++;
				else											// long
					m_p2d_LongCount++;
			}
			else 												// Формат плавающей точки
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float
					m_p2d_FloatCount++;
				else 											// double
					m_p2d_DoubleCount++;
			}
		}
		else 													// Метрика в 3D
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Целый тип
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// short/float, где float - высота
					m_p3d_ShortCount++;
				else 											// long/float
					m_p3d_LongCount++;
			}
			else 												// Плавающая точка
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float/float
					m_p3d_FloatCount++;
				else 											// double/double
					m_p3d_DoubleCount++;
			}
		}
		currentPosition += m_ptr_headers[i]->length;	
	}
}
/////////////////////////////////////////////////////////////////////
void cMap::AllocMem4Points()
{
	if (m_p2d_ShortCount > 0)
		m_p2d_Short = new s2dPoint<short>*[m_p2d_ShortCount];
	if (m_p2d_LongCount > 0)
		m_p2d_Long = new s2dPoint<long>*[m_p2d_LongCount];
	if (m_p2d_FloatCount > 0)
		m_p2d_Float = new s2dPoint<float>*[m_p2d_FloatCount];
	if (m_p2d_DoubleCount > 0)
		m_p2d_Double = new s2dPoint<double>*[m_p2d_DoubleCount];
	if (m_p3d_ShortCount > 0)
		m_p3d_Short = new s3dPoint<short, float>*[m_p3d_ShortCount];
	if (m_p3d_LongCount > 0)
		m_p3d_Long = new s3dPoint<long, float>*[m_p3d_LongCount];
	if (m_p3d_FloatCount > 0)
		m_p3d_Float = new s3dPoint<float, float>*[m_p3d_FloatCount];
	if (m_p3d_DoubleCount > 0)
		m_p3d_Double = new s3dPoint<double, double>*[m_p3d_DoubleCount];
}
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
void cMap::setPointers()
{
	// Количество байт данных карты, которые уже были интерпретированы:
	unsigned long currentPosition = sizeof(sPassport) + sizeof(sDescriptor);

	// Итераторы для заполнения разных массивов метрик:
	short iter_2dS = 0;
	short iter_2dL = 0;
	short iter_2dF = 0;
	short iter_2dD = 0;
	short iter_3dS = 0;
	short iter_3dL = 0;
	short iter_3dF = 0;
	short iter_3dD = 0;

	// Цикл по количеству записей в карте
	for(unsigned long i = 0; i < m_ptr_descriptor->recordCount; i ++)	
	{


		if (m_ptr_headers[i]->flag_dimension == 0)				// Определение размерности метрики (2D/3D)
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Определение типа данных метрика (Целое/Плав.т.)
			{						
				if(m_ptr_headers[i]->flag_metricSize == 0)		// Определение длины типа данных (2-4 B/4-8 B)
					m_p2d_Short[iter_2dS++] = reinterpret_cast<s2dPoint<short>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
				else											// long
					m_p2d_Long[iter_2dL++] = reinterpret_cast<s2dPoint<long>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
			}
			else 												// Формат плавающей точки
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float
				{
					m_p2d_Float[iter_2dF++] = reinterpret_cast<s2dPoint<float>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
					// cout << m_p2d_Float[iter_2dF - 1]->X << "\n";
				}

				else 											// double
					m_p2d_Double[iter_2dD++] = reinterpret_cast<s2dPoint<double>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
			}
		}
		else 													// Метрика в 3D
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Целый тип
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// short/float, где float - высота
					m_p3d_Short[iter_3dS++] = reinterpret_cast<s3dPoint<short, float>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
				else 											// long/float
					m_p3d_Long[iter_3dL++] = reinterpret_cast<s3dPoint<long, float>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
			}
			else 												// Плавающая точка
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float/float
					m_p3d_Float[iter_3dF++] = reinterpret_cast<s3dPoint<float, float>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
				else 											// double/double
					m_p3d_Double[iter_3dD++] = reinterpret_cast<s3dPoint<double, double>*>(m_ptr_charMap +
					 currentPosition + sizeof(sHeader));
			}
		}


		if (m_ptr_records[i].header->subobjectCount > 0)	// Ищем подобъекты если они есть
		{
			cout << "======Record #" << i << "======\n";
			// Вылеляем место под subobjectCount указателей:
			m_ptr_records[i].subobjectPoints = new void*[m_ptr_records[i].header->subobjectCount];
			 // Определям размер точки (в байтах)
			short pointsSize = getPointsSize(i);

			// cout << "subObjectCount = " << m_ptr_records[i].header->subobjectCount << "\n";
			// Сумма размеров метрик подобъектов которые уже интерпретированы как точки:
			unsigned long subPosition = 0; 
			for (int j = 0; j < m_ptr_records[i].header->subobjectCount; j++)
			{
				cout << "subObject #" << j << "\n";
				// указатель на служебное поле, содержащее количество точек подобъекта:				
				unsigned short* pCount = reinterpret_cast<unsigned short*>
				(	m_ptr_charMap +		// Указатель на массив с бинарной картой
					currentPosition + 	// Количество байт, которое уже было прочитано до текущей записи
					sizeof(sHeader) + 	// Размер заголовка, с которого начиается запись 
					m_ptr_records[i].header->metricPointsCount*pointsSize +	// Размер метрики записи
					sizeof(short) + 	// Служебное поле с резервом, которое находится после метрики
					subPosition);		// Сумма размеров метрик уже пройденных подобъектов

				m_ptr_records[i].subobjectPoints[j] = reinterpret_cast<unsigned short*>
				(	m_ptr_charMap + 
				 	currentPosition +
				 	sizeof(sHeader) +
				 	m_ptr_records[i].header->metricPointsCount*2*sizeof(float) +
				 	2*sizeof(short) +	// Служебное поле с резервом + поле с количеством точек
				 	subPosition);

				cout << "Points count = " << *pCount << "\n";

				// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
				subPosition += pointsSize * (*pCount) + 2*sizeof(short); 
			}
		}
		// Увеличваем счетчик интерпретированных байт на размер размер записи
		currentPosition += m_ptr_headers[i]->length;	
	}
	cout << "Pointers was setted\n";
}
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
short cMap::writePassportLog(char* p_ptr_filename)
{
	ofstream outfile;
	outfile.open(p_ptr_filename);
	if (!outfile){	
		cerr << "Unable to write passport log file!\n";	
		outfile.close();
		return 0;
	}

	outfile << 
	// General data
	 	hex	<< "id = "					<< m_ptr_passport->id					<< "\n"
	<<	dec	<< "length = "				<< m_ptr_passport->length				<< "\n"
	<<	hex	<< "edition = "				<< m_ptr_passport->edition				<< "\n"
	<<	dec	<< "checkSum = "			<< m_ptr_passport->checkSum				<< "\n"
			<< "creationgDate = "		<< m_ptr_passport->creationgDate		<< "\n"
			<< "nomenclature = "		<< m_ptr_passport->nomenclature			<< "\n"
			<< "scale = "				<< m_ptr_passport->scale				<< "\n"
			<< "sheetName = "			<< m_ptr_passport->sheetName			<< "\n\n"
	// Info flags
			<< "flag_status = "			<< static_cast<unsigned short>(m_ptr_passport->flag_status)			<< "\n"
			<< "flag_precision = "		<< static_cast<unsigned short>(m_ptr_passport->flag_projection)		<< "\n"
			<< "flag_realCoordinate = "	<< static_cast<unsigned short>(m_ptr_passport->flag_realCoordinate)	<< "\n"
			<< "flag_codingType = "		<< static_cast<unsigned short>(m_ptr_passport->flag_codingType)		<< "\n"
			<< "flag_generalTable = "	<< static_cast<unsigned short>(m_ptr_passport->flag_generalTable)	<< "\n"
			<< "flag_signature = "		<< static_cast<unsigned short>(m_ptr_passport->flag_signature)		<< "\n"
			<< "flag_precision = "		<< static_cast<unsigned short>(m_ptr_passport->flag_precision)		<< "\n"
			<< "flag_specSort = "		<< static_cast<unsigned short>(m_ptr_passport->flag_specSort)		<< "\n"
			<< "flag_reserveFlag = "	<< static_cast<unsigned short>(m_ptr_passport->flag_reserve)		<< "\n\n"

			<< "classifier = "			<< m_ptr_passport->classifier			<< "\n\n"
	// Rectangular coordinates of sheet corners
			<< "XSouthWestCorner = "	 << m_ptr_passport->XSouthWestCorner	<< "\n"
			<< "YSouthWestCorner = "	 << m_ptr_passport->YSouthWestCorner	<< "\n"
			<< "XNorthWestCorner = "	 << m_ptr_passport->XNorthWestCorner	<< "\n"
			<< "YNorthWestCorner = "	 << m_ptr_passport->YNorthWestCorner	<< "\n"
			<< "XNorthEastCorner = "	 << m_ptr_passport->XNorthEastCorner	<< "\n"
			<< "YNorthEastCorner = "	 << m_ptr_passport->YNorthEastCorner	<< "\n"
			<< "XSouthEastCorner = "	 << m_ptr_passport->XSouthEastCorner	<< "\n"
			<< "YSouthEastCorner = "	 << m_ptr_passport->YSouthEastCorner	<< "\n\n"
	// Geodetic coordinates of sheet corners
			<< "BSouthWestCorner = "	<< m_ptr_passport->BSouthWestCorner		<< "\n"
			<< "LSouthWestCorner = "	<< m_ptr_passport->LSouthWestCorner		<< "\n"
			<< "BNorthWestCorner = "	<< m_ptr_passport->BNorthWestCorner		<< "\n"
			<< "LNorthWestCorner = "	<< m_ptr_passport->LNorthWestCorner		<< "\n"
			<< "BNorthEastCorner = "	<< m_ptr_passport->BNorthEastCorner		<< "\n"
			<< "LNorthEastCorner = "	<< m_ptr_passport->LNorthEastCorner		<< "\n"
			<< "BSouthEastCorner = "	<< m_ptr_passport->BSouthEastCorner		<< "\n"
			<< "LSouthEastCorner = "	<< m_ptr_passport->LSouthEastCorner		<< "\n\n"
	// Mathematical basis of sheet
			<< "ellipsoidType = "		<< static_cast<unsigned short>(m_ptr_passport->ellipsoidType)		<< "\n"
			<< "elevationSystem = "		<< static_cast<unsigned short>(m_ptr_passport->elevationSystem)		<< "\n"
			<< "materialProjection = "	<< static_cast<unsigned short>(m_ptr_passport->materialProjection)	<< "\n"
			<< "coordinateSystem = "	<< static_cast<unsigned short>(m_ptr_passport->coordinateSystem)	<< "\n"
			<< "planeUnit = "			<< static_cast<unsigned short>(m_ptr_passport->planeUnit)			<< "\n"
			<< "heightUnit = "			<< static_cast<unsigned short>(m_ptr_passport->heightUnit)			<< "\n"
			<< "frameType = "			<< static_cast<unsigned short>(m_ptr_passport->frameType)			<< "\n"
			<< "mapType = "				<< static_cast<unsigned short>(m_ptr_passport->mapType)				<< "\n\n"
	// Reference data on source material
			<< "updateDate = "			<< m_ptr_passport->updateDate			<< "\n"
			<< "materialKind = "		<< static_cast<unsigned short>(m_ptr_passport->materialKind)		<< "\n"
			<< "materialType = "		<< static_cast<unsigned short>(m_ptr_passport->materialType)		<< "\n"
			<< "reserveRef1 = "			<< m_ptr_passport->reserveRef1			<< "\n"
			<< "magneticDeclination = "	<< m_ptr_passport->magneticDeclination	<< "\n"
			<< "avgMeridianConv = "		<< m_ptr_passport->avgMeridianConv		<< "\n"
			<< "magneticChange = "		<< m_ptr_passport->magneticChange		<< "\n"
			<< "declinationUpdt = "		<< m_ptr_passport->declinationUpdt		<< "\n"
			<< "reserveRef2 = "			<< m_ptr_passport->reserveRef2			<< "\n"
			<< "reliefSection = "		<< m_ptr_passport->reliefSection		<< "\n"
			<< "reserve = "				<< m_ptr_passport->reserve				<< "\n"
			<< "deviceResolution = "	<< m_ptr_passport->deviceResolution		<< "\n\n"
	// Location of the frame on the device
			<< "X_SW_FrameLoacation = "	<< m_ptr_passport->X_SW_FrameLoacation	<< "\n"
			<< "Y_SW_FrameLoacation = "	<< m_ptr_passport->Y_SW_FrameLoacation	<< "\n"
			<< "X_NW_FrameLoacation = "	<< m_ptr_passport->X_NW_FrameLoacation	<< "\n"
			<< "Y_NW_FrameLoacation = "	<< m_ptr_passport->Y_NW_FrameLoacation	<< "\n"
			<< "X_NE_FrameLoacation = "	<< m_ptr_passport->X_NE_FrameLoacation	<< "\n"
			<< "Y_NE_FrameLoacation = "	<< m_ptr_passport->Y_NE_FrameLoacation	<< "\n"
			<< "X_SE_FrameLoacation = "	<< m_ptr_passport->X_SE_FrameLoacation	<< "\n"
			<< "Y_SE_FrameLoacation = "	<< m_ptr_passport->Y_SE_FrameLoacation	<< "\n\n"
	// Reference data on the projection of the source material	
			<< "firstMainParallel = "	<< m_ptr_passport->firstMainParallel	<< "\n"
			<< "secondMainParallel = "	<< m_ptr_passport->secondMainParallel	<< "\n"
			<< "axialMeridian = "		<< m_ptr_passport->axialMeridian		<< "\n"
			<< "mainPointParallel = "	<< m_ptr_passport->mainPointParallel	<< "\n"
			<< "poleLatitude = "		<< m_ptr_passport->poleLatitude			<< "\n"
			<< "poleLongitude = "		<< m_ptr_passport->poleLongitude		<< "\n";
	cout << "Passport was recored in " << p_ptr_filename << "\n";
	outfile.close();
	return 1;
}
////////////////////////////////////////////////////////////////////
short cMap::writeDescriptorLog(char* p_ptr_filename)
{
	ofstream outfile;
	outfile.open(p_ptr_filename);
	if (!outfile){	
		cerr << "Unable to write descriptor log file!\n";	
		outfile.close();
		return 0;
	}

	outfile << 
	 	hex	<< "id = "					<< m_ptr_descriptor->id					<< "\n"
	<<	dec	<< "length = "				<< m_ptr_descriptor->length				<< "\n"
			<< "nomenclature = "		<< m_ptr_descriptor->nomenclature		<< "\n"
			<< "recodrCount = "			<< m_ptr_descriptor->recordCount		<< "\n\n"

			<< "flag_status = "			<< static_cast<unsigned short>(m_ptr_descriptor->flag_status)			<< "\n"
			<< "flag_projection = "		<< static_cast<unsigned short>(m_ptr_descriptor->flag_projection)		<< "\n"
			<< "flag_realCoordinate = "	<< static_cast<unsigned short>(m_ptr_descriptor->flag_realCoordinate)	<< "\n"
			<< "flag_codingType = "		<< static_cast<unsigned short>(m_ptr_descriptor->flag_codingType)		<< "\n"
			<< "flag_generalTable = "	<< static_cast<unsigned short>(m_ptr_descriptor->flag_generalTable)		<< "\n"
			<< "flag_signature = "		<< static_cast<unsigned short>(m_ptr_descriptor->flag_signature)		<< "\n"
			<< "flag_reserve = "		<< static_cast<unsigned short>(m_ptr_descriptor->flag_reserve)			<< "\n\n"

			<< "objectsClassifier = "	<< m_ptr_descriptor->objectsClassifier	<< "\n"
			<< "semanticsClassifier = "	<< m_ptr_descriptor->semanticsClassifier<< "\n\n";
	cout << "Descriptor was recored in " << p_ptr_filename << "\n";
	outfile.close();
	return 1;
}
/////////////////////////////////////////////////////////////////////
short cMap::writeHeadersLog(char* p_ptr_filename)
{
	ofstream outfile;
	outfile.open(p_ptr_filename);
	if (!outfile){	
		cerr << "Unable to write descriptor log file!\n";	
		outfile.close();
		return 0;
	}
	for (int i = 0; i < m_ptr_descriptor->recordCount; i++)
	{
	outfile << "============================================="			 		<< "\n" 
			<< "Record Header [" 		<< i << "]"								<< "\n"
			<< "============================================="					<< "\n"	
	<<	hex	<< "id = "						<< m_ptr_headers[i]->id				<< "\n"
	<<	dec	<< "length = "					<< m_ptr_headers[i]->length			<< "\n"
			<< "metricLenght = "			<< m_ptr_headers[i]->metricLenght		<< "\n"
			<< "classificationCode = "		<< m_ptr_headers[i]->classificationCode<< "\n\n"

			<< "numberInGroup = "			<< m_ptr_headers[i]->numberInGroup		<< "\n"
			<< "groupNumber = "				<< m_ptr_headers[i]->groupNumber		<< "\n\n"

			<< "flag_localization = "		<< m_ptr_headers[i]->flag_localization	<< "\n"
			<< "flag_frameExit = "			<< m_ptr_headers[i]->flag_frameExit	<< "\n"
			<< "flag_insularity = "			<< m_ptr_headers[i]->flag_insularity	<< "\n"
			<< "flag_semantics = "			<< m_ptr_headers[i]->flag_semantics	<< "\n"
			<< "flag_metricSize = "			<< m_ptr_headers[i]->flag_metricSize	<< "\n"
			<< "flag_bindingVector = "		<< m_ptr_headers[i]->flag_bindingVector<< "\n"
			<< "flag_UNICODE = "			<< m_ptr_headers[i]->flag_UNICODE		<< "\n"
			<< "flag_reserve = "			<< m_ptr_headers[i]->flag_reserve		<< "\n\n"

			<< "flag_metricFormat = "		<< m_ptr_headers[i]->flag_metricFormat	<< "\n"
			<< "flag_dimension = "			<< m_ptr_headers[i]->flag_dimension	<< "\n"
			<< "flag_metricType = "			<< m_ptr_headers[i]->flag_metricType	<< "\n"
			<< "flag_metricText = "			<< m_ptr_headers[i]->flag_metricText	<< "\n"
			<< "flag_sign = "				<< m_ptr_headers[i]->flag_sign			<< "\n"
			<< "flag_scalableGraphics = "	<< m_ptr_headers[i]->flag_scalableGraphics<< "\n"
			<< "flag_metricSpline = "		<< m_ptr_headers[i]->flag_metricSpline	<< "\n\n"

			<< "bottomBorder = "			<< m_ptr_headers[i]->bottomBorder		<< "\n"
			<< "topBorder = "				<< m_ptr_headers[i]->topBorder			<< "\n\n"

			<< "metricPointsCountBig = "	<< m_ptr_headers[i]->metricPointsCountBig<< "\n"
			<< "subobjectNumber = "			<< m_ptr_headers[i]->subobjectCount	<< "\n"
			<< "metricPointsCount = "		<< m_ptr_headers[i]->metricPointsCount	<< "\n\n";
	}
	cout << "Headers was recored in " << p_ptr_filename << "\n";
	outfile.close();
	return 0;
}
/////////////////////////////////////////////////////////////////////
short cMap::writeRecordMetricLog(char* p_ptr_filename)
{
	ofstream outfile;
	outfile.open(p_ptr_filename);
	if (!outfile){	
		cerr << "Unable to write descriptor log file!\n";	
		outfile.close();
		return 0;
	}

	outfile << "======= 2D SHORT POINTS ========\n"; 
	for (int i = 0; i < m_p2d_ShortCount; i ++)
		outfile << m_p2d_Short[i]->X << " - " << m_p2d_Short[i]->Y << "\n";
	outfile << "======= 2D LONG POINTS ========\n";
	for (int i = 0; i < m_p2d_LongCount; i ++)
		outfile << m_p2d_Long[i]->X << " - " << m_p2d_Long[i]->Y << "\n";
	outfile << "======= 2D FLOAT POINTS ========\n";
	for (int i = 0; i < m_p2d_FloatCount; i ++)
		outfile << m_p2d_Float[i]->X << " - " << m_p2d_Float[i]->Y << "\n";
	outfile << "======= 2D DOUBLE POINTS ========\n";
	for (int i = 0; i < m_p2d_DoubleCount; i ++)
		outfile << m_p2d_Double[i]->X << " - " << m_p2d_Double[i]->Y << "\n";
	outfile << "======= 3D SHORT POINTS ========\n";
	for (int i = 0; i < m_p3d_ShortCount; i ++)
		outfile << m_p3d_Short[i]->X << " - " << m_p3d_Short[i]->Y << "\n";
	outfile << "======= 3D LONG POINTS ========\n";
	for (int i = 0; i < m_p3d_LongCount; i ++)
		outfile << m_p3d_Long[i]->X << " - " << m_p3d_Long[i]->Y << "\n";
	outfile << "======= 3D FLOAT POINTS ========\n";
	for (int i = 0; i < m_p3d_FloatCount; i ++)
		outfile << m_p3d_Float[i]->X << " - " << m_p3d_Float[i]->Y << "\n";
	outfile << "======= 3D DOUBLE POINTS ========\n";
	for (int i = 0; i < m_p3d_DoubleCount; i ++)
		outfile << m_p3d_Double[i]->X << " - " << m_p3d_Double[i]->Y << "\n";



	cout << "Metric was recored in " << p_ptr_filename << "\n";
	outfile.close();
	return 1;
}
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
void cMap::showPCounts()
{
	cout 	<< m_p2d_ShortCount		<< "\n"
			<< m_p2d_LongCount 		<< "\n"
			<< m_p2d_FloatCount		<< "\n"
			<< m_p2d_DoubleCount	<< "\n"
			<< m_p3d_ShortCount		<< "\n"
			<< m_p3d_LongCount		<< "\n"
			<< m_p3d_FloatCount		<< "\n"
			<< m_p3d_DoubleCount	<< "\n";
}
