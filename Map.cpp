//Map.cpp
#include <iostream>
#include "Map.h"

using namespace std;

cMap::cMap():m_ptr_charMap(NULL), m_ptr_passport(NULL), m_ptr_descriptor(NULL), m_ptr_records(NULL){}
/////////////////////////////////////////////////////////////////////
cMap::~cMap()
{
	delete[] m_ptr_charMap;
	delete[] m_ptr_records;
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

void cMap::setPointsType(long i)
{
	if (m_ptr_records[i].is2dPoint())				// Определение размерности метрики (2D/3D)
	{
		if (m_ptr_records[i].isInteger())			// Определение типа данных метрика (Целое/Плав.т.)
		{						
			if(m_ptr_records[i].isShortSize())		// Определение длины типа данных (2-4 B/4-8 B)
				m_ptr_records[i].pointsType = mType_2dShort;
			else									// long
				m_ptr_records[i].pointsType = mType_2dLong;
		}
		else 										// Формат плавающей точки
		{
			if(m_ptr_records[i].isShortSize())		// float
				m_ptr_records[i].pointsType = mType_2dFloat;
			else 									// double
				m_ptr_records[i].pointsType = mType_2dDouble;
		}
	}
	else 											// Метрика в 3D
	{
		if (m_ptr_records[i].isInteger())			// Целый тип
		{
			if(m_ptr_records[i].isShortSize())		// short/float, где float - высота
				m_ptr_records[i].pointsType = mType_3dShort;
			else 									// long/float
				m_ptr_records[i].pointsType = mType_3dLong;
		}
		else 										// Плавающая точка
		{
			if(m_ptr_records[i].isShortSize())		// float/float
				m_ptr_records[i].pointsType = mType_3dFloat;
			else 									// double/double
				m_ptr_records[i].pointsType = mType_3dDouble;
		}
	}
}


/////////////////////////////////////////////////////////////////////
void cMap::setPointers()
{
	// Паспорт в карте записан первым -> устанавливаем указатель паспотра m_ptr_passport
	// в начало считанной сырой карты:
	m_ptr_passport = reinterpret_cast<sPassport*>		(m_ptr_charMap);
	// Сразу после паспорта записан дескриптор -> устанавливаем указатель дескриптора
	// в позицию (начала карты + размер паспорта), т.е. после паспорта:
	m_ptr_descriptor = reinterpret_cast<sDescriptor*>	(m_ptr_charMap + sizeof(sPassport));
	// В дескрипторе хранится информация о количестве записей карты.
	// Выделяем место под записи:
	m_ptr_records = new sRecord[m_ptr_descriptor->recordCount];	

	unsigned long ul_bufPosition;	// Количество байт сырых данных карты, которые
									// уже были интерпретированы как полезные данные

	// Первые 452 байта пасспорта и дескриптора уже были интерпретированы:
	ul_bufPosition = sizeof(sPassport) + sizeof(sDescriptor);

	// Цикл по количеству записей в карте
	for(unsigned long i = 0; i < m_ptr_descriptor->recordCount; i ++)	
	{
		m_ptr_records[i].header = reinterpret_cast<sHeader*>	(m_ptr_charMap + ul_bufPosition);
		m_ptr_records[i].points = 								(m_ptr_charMap + ul_bufPosition + sizeof(sHeader));
		setPointsType(i); 		// Определям тип точек

		if (m_ptr_records[i].header->subobjectCount > 0)	// Ищем подобъекты если они есть
		{
			unsigned long ul_bufPosition4SO = 0;
			cout << "subObjectCount = " << m_ptr_records[i].header->subobjectCount << "\n";
			for (int j = 0; j < m_ptr_records[i].header->subobjectCount; j++)
			{
				cout << "subObject #" << j << "\n";
				unsigned short* pCount = reinterpret_cast<unsigned short*>(m_ptr_charMap + ul_bufPosition + sizeof(sHeader) + m_ptr_records[i].header->metricPointsCount*2*sizeof(float) + sizeof(short));
				cout << "Points count = " << *pCount << "\n";
			}
		}
		// Увеличваем счетчик интерпретированных байт на размер размер записи
		ul_bufPosition += m_ptr_records[i].header->length;	
	}
	cout << "Pointers was setted\n";
}
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
void cMap::readOneRecordMetric(long &p_recordNo, unsigned long &p_bufPosition)
{
	if (m_ptr_records[p_recordNo].header->subobjectCount >= 1 )
	{
		unsigned short* pCount = reinterpret_cast<unsigned short*>(m_ptr_charMap + p_bufPosition + sizeof(sHeader) + m_ptr_records[p_recordNo].header->metricLenght + sizeof(short));
		// dec2bin(*pCount);
		cout << (unsigned long)*pCount;
		cout << endl;
	}
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
	if (m_ptr_records == NULL)
	{
		cout << "ERROR. Records was not read! Headers are not recorded!\n";
		return -1;
	}

	ofstream outfile;
	outfile.open(p_ptr_filename);
	if (!outfile){	
		cerr << "Unable to write descriptor log file!\n";	
		outfile.close();
		return 0;
	}
	for (int i = 0; i < m_ptr_descriptor->recordCount; i++)
	// for (int i = 0; i < 50; i++)
	{
	outfile << "============================================="			 		<< "\n" 
			<< "Record Header [" 		<< i << "]"								<< "\n"
			<< "============================================="					<< "\n"	
	<<	hex	<< "id = "						<< m_ptr_records[i].header->id					<< "\n"
	<<	dec	<< "length = "					<< m_ptr_records[i].header->length				<< "\n"
			<< "metricLenght = "			<< m_ptr_records[i].header->metricLenght		<< "\n"
			<< "classificationCode = "		<< m_ptr_records[i].header->classificationCode	<< "\n\n"

			<< "numberInGroup = "			<< m_ptr_records[i].header->numberInGroup		<< "\n"
			<< "groupNumber = "				<< m_ptr_records[i].header->groupNumber			<< "\n\n"

			<< "flag_localization = "		<< m_ptr_records[i].header->flag_localization	<< "\n"
			<< "flag_frameExit = "			<< m_ptr_records[i].header->flag_frameExit		<< "\n"
			<< "flag_insularity = "			<< m_ptr_records[i].header->flag_insularity		<< "\n"
			<< "flag_semantics = "			<< m_ptr_records[i].header->flag_semantics		<< "\n"
			<< "flag_metricSize = "			<< m_ptr_records[i].header->flag_metricSize	<< "\n"
			<< "flag_bindingVector = "		<< m_ptr_records[i].header->flag_bindingVector	<< "\n"
			<< "flag_UNICODE = "			<< m_ptr_records[i].header->flag_UNICODE		<< "\n"
			<< "flag_reserve = "			<< m_ptr_records[i].header->flag_reserve		<< "\n\n"

			<< "flag_metricFormat = "		<< m_ptr_records[i].header->flag_metricFormat	<< "\n"
			<< "flag_dimension = "			<< m_ptr_records[i].header->flag_dimension		<< "\n"
			<< "flag_metricType = "		<< m_ptr_records[i].header->flag_metricType	<< "\n"
			<< "flag_metricText = "			<< m_ptr_records[i].header->flag_metricText		<< "\n"
			<< "flag_sign = "				<< m_ptr_records[i].header->flag_sign			<< "\n"
			<< "flag_scalableGraphics = "	<< m_ptr_records[i].header->flag_scalableGraphics<< "\n"
			<< "flag_metricSpline = "		<< m_ptr_records[i].header->flag_metricSpline	<< "\n\n"

			<< "bottomBorder = "			<< m_ptr_records[i].header->bottomBorder		<< "\n"
			<< "topBorder = "				<< m_ptr_records[i].header->topBorder			<< "\n\n"

			<< "metricPointsCountBig = "	<< m_ptr_records[i].header->metricPointsCountBig<< "\n"
			<< "subobjectNumber = "			<< m_ptr_records[i].header->subobjectCount		<< "\n"
			<< "metricPointsCount = "		<< m_ptr_records[i].header->metricPointsCount	<< "\n\n";
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


	for(unsigned long i = 0; i < m_ptr_descriptor->recordCount; i++) // Цикл по всем записям
	{
		// Определение количетва точек в записи: обычное или большое число точек?
		unsigned long pointsCount;
		if (m_ptr_records[i].header->metricPointsCount < 65535)		
			pointsCount = m_ptr_records[i].header->metricPointsCount;
		else
			pointsCount = m_ptr_records[i].header->metricPointsCountBig;

		outfile << "==============Metric " << i << " =================\n";

		if (m_ptr_records[i].is2dPoint())						// Определение размерности метрики (2D/3D)
		{
			if (m_ptr_records[i].isInteger())					// Определение типа данных метрика (Целое/Плав.т.)
			{						
				if(m_ptr_records[i].isShortSize())				// Определение длины типа данных (2-4 B/4-8 B)
				{
					//Заменить на функцию!
					s2dPoint<short>* tmpPoints = reinterpret_cast<s2dPoint<short>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
				else											// long
				{
					s2dPoint<long>* tmpPoints = reinterpret_cast<s2dPoint<long>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
			}
			else 												// Формат плавающей точки
			{
				if(m_ptr_records[i].isShortSize())				// float
				{
					s2dPoint<float>* tmpPoints = reinterpret_cast<s2dPoint<float>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
				else 											// double
				{
					s2dPoint<double>* tmpPoints = reinterpret_cast<s2dPoint<double>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
					// cout << "i = " << i << "\n";
				}
			}
		}
		else 		// Метрика в 3D
		{
			if (m_ptr_records[i].isInteger())					// Целый тип
			{
				if(m_ptr_records[i].isShortSize())				// short/float, где float - высота
				{
					s3dPoint<short, float>* tmpPoints = reinterpret_cast<s3dPoint<short, float>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
				else 											// long/float
				{
					s3dPoint<long, float>* tmpPoints = reinterpret_cast<s3dPoint<long, float>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
			}
			else 												// Плавающая точка
			{
				if(m_ptr_records[i].isShortSize())				// float/float
				{
					s3dPoint<float, float>* tmpPoints = reinterpret_cast<s3dPoint<float, float>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
				else 											// double/double
				{
					s3dPoint<float, double>* tmpPoints = reinterpret_cast<s3dPoint<float, double>*>(m_ptr_records[i].points);
					for (unsigned long j = 0; j < pointsCount; j++)
					{
						outfile << tmpPoints[j].X << 
						" - " 	<< tmpPoints[j].Y << "\n";
					}
				}
			}
		}
	}

	cout << "Metric was recored in " << p_ptr_filename << "\n";
	outfile.close();
	return 1;
}
/////////////////////////////////////////////////////////////////////
