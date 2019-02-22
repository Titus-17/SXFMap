//Map.cpp
#include <iostream>
#include "Map.h"

using namespace std;

cMap::cMap()
{
	m_ptr_charMap 		= NULL;
	m_ptr_passport 		= NULL;
	m_ptr_descriptor 	= NULL;

	m_p2d_shortCount	= 0;
	m_p2d_longCount		= 0;
	m_p2d_floatCount	= 0;
	m_p2d_doubleCount	= 0;
	m_p3d_shortCount	= 0;
	m_p3d_longCount		= 0;
	m_p3d_floatCount	= 0;
	m_p3d_doubleCount	= 0;

	m_p2d_shortCountSO	= 0;
	m_p2d_longCountSO	= 0;
	m_p2d_floatCountSO	= 0;
	m_p2d_doubleCountSO	= 0;
	m_p3d_shortCountSO	= 0;
	m_p3d_longCountSO	= 0;
	m_p3d_floatCountSO	= 0;
	m_p3d_doubleCountSO	= 0;

	m_ptr_records_short_2d 	= NULL;
	m_ptr_records_long_2d 	= NULL;
	m_ptr_records_float_2d	= NULL;
	m_ptr_records_double_2d	= NULL;
	m_ptr_records_short_3d 	= NULL;
	m_ptr_records_long_3d 	= NULL;
	m_ptr_records_float_3d 	= NULL;
	m_ptr_records_double_3d = NULL;

	m_p2d_ShortSO 		= NULL;
	m_p2d_LongSO		= NULL;
	m_p2d_FloatSO 		= NULL;
	m_p2d_DoubleSO		= NULL;
	m_p3d_ShortSO 		= NULL;
	m_p3d_LongSO		= NULL;
	m_p3d_FloatSO 		= NULL;
	m_p3d_DoubleSO 		= NULL;
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

	// if (m_p2d_shortCount > 0)
	// 	delete[] m_p2d_Short;
	// if (m_p2d_longCount > 0)
	// 	delete[] m_p2d_Long;
	// if (m_p2d_floatCount > 0)
	// 	delete[] m_p2d_Float;
	// if (m_p2d_doubleCount > 0)
	// 	delete[] m_p2d_Double;
	// if (m_p3d_shortCount > 0)
	// 	delete[] m_p3d_Short;
	// if (m_p3d_longCount > 0)
	// 	delete[] m_p3d_Long;
	// if (m_p3d_floatCount > 0)
	// 	delete[] m_p3d_Float;
	// if (m_p3d_doubleCount > 0)
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
void cMap::calcPointsCount()
{
	m_ptr_passport 		= reinterpret_cast<sPassport*>		(m_ptr_charMap);
	m_ptr_descriptor 	= reinterpret_cast<sDescriptor*>	(m_ptr_charMap + sizeof(sPassport));
	m_ptr_headers 		= new sHeader*[m_ptr_descriptor->recordCount];	

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
				{
					m_p2d_shortCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p2d_shortCountSO++;
				}
				else											// long
				{
					m_p2d_longCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p2d_longCountSO++;
				}
			}
			else 												// Формат плавающей точки
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float
				{
					m_p2d_floatCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p2d_floatCountSO++;
				}
				else 											// double
				{
					m_p2d_doubleCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p2d_doubleCountSO++;
				}
			}
		}
		else 													// Метрика в 3D
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Целый тип
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// short/float, где float - высота
				{
					m_p3d_shortCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p3d_shortCountSO++;
				}
				else 											// long/float
				{
					m_p3d_longCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p3d_longCountSO++;
				}
			}
			else 												// Плавающая точка
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float/float
				{
					m_p3d_floatCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p3d_floatCountSO++;
				}
				else 											// double/double
				{
					m_p3d_doubleCount++;
					if (m_ptr_headers[i]->subobjectCount > 0)
						m_p3d_doubleCountSO++;
				}
			}
		}
		currentPosition += m_ptr_headers[i]->length;	
	}
}
/////////////////////////////////////////////////////////////////////
void cMap::AllocMem4Points()
{
	if (m_p2d_shortCount > 0)
		m_ptr_records_short_2d 	= new sМetric_2d	<short>				[m_p2d_shortCount];
	if (m_p2d_longCount > 0)
		m_ptr_records_long_2d 	= new sМetric_2d	<long>				[m_p2d_longCount];
	if (m_p2d_floatCount > 0)
		m_ptr_records_float_2d 	= new sМetric_2d	<float>				[m_p2d_floatCount];
	if (m_p2d_doubleCount > 0)
		m_ptr_records_double_2d = new sМetric_2d	<double>			[m_p2d_doubleCount];
	if (m_p3d_shortCount > 0)
		m_ptr_records_short_3d 	= new sMetric_3d	<short, float>		[m_p3d_shortCount];
	if (m_p3d_longCount > 0)
		m_ptr_records_long_3d 	= new sMetric_3d	<long, float>		[m_p3d_longCount];
	if (m_p3d_floatCount > 0)
		m_ptr_records_float_3d 	= new sMetric_3d	<float, float>		[m_p3d_floatCount];
	if (m_p3d_doubleCount > 0)
		m_ptr_records_double_3d = new sMetric_3d	<double, double>	[m_p3d_doubleCount];

	if (m_p2d_shortCountSO > 0)
		m_p2d_ShortSO = new sМetricSO_2d	<short>		[m_p2d_shortCountSO];
	if (m_p2d_longCountSO > 0)
		m_p2d_LongSO = new sМetricSO_2d		<long>		[m_p2d_longCountSO];
	if (m_p2d_floatCountSO > 0)
		m_p2d_FloatSO = new sМetricSO_2d	<float>		[m_p2d_floatCountSO];
	if (m_p2d_doubleCountSO > 0)
		m_p2d_DoubleSO = new sМetricSO_2d	<double>	[m_p2d_doubleCountSO];
	if (m_p3d_shortCountSO > 0)
		m_p3d_ShortSO = new sMetricSO_3d	<short, float>		[m_p3d_shortCountSO];
	if (m_p3d_longCountSO > 0)
		m_p3d_LongSO = new sMetricSO_3d		<long, float>		[m_p3d_longCountSO];
	if (m_p3d_floatCountSO > 0)
		m_p3d_FloatSO = new sMetricSO_3d	<float, float>		[m_p3d_floatCountSO];
	if (m_p3d_doubleCountSO > 0)
		m_p3d_DoubleSO = new sMetricSO_3d	<double, double>	[m_p3d_doubleCountSO];
}
/////////////////////////////////////////////////////////////////////




// ===============================================================
// Когда-нибудь ЭТО превратится в шаблон, но только не сегодня...
// ===============================================================

template <class T>
void cMap::findSOmetric2d(unsigned short &p_metricNum, unsigned short &p_subObjNum, sМetric_2d<T> *p_metric, sМetricSO_2d<T> *p_metricSO, unsigned long &p_recordNum, unsigned long &curPos)
{
	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
	p_metricSO[p_subObjNum].header = p_metric[p_metricNum].header;
	p_metricSO[p_subObjNum].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
	p_metricSO[p_subObjNum].p2d_points = new s2dPoint<T>*[m_ptr_headers[p_recordNum]->subobjectCount];
	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*2*sizeof(T) + sizeof(short);

	for (int i = 0; i < m_ptr_headers[p_recordNum]->subobjectCount; i++)
	{
		// Указатель на служебное поле, содержащее количество точек подобъекта:				
		p_metricSO[p_subObjNum].pointsCount[i] = reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		
		p_metricSO[p_subObjNum].p2d_points[i] 	= reinterpret_cast<s2dPoint<T>*>	(m_ptr_charMap + subPosition + sizeof(short));

		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
		subPosition += 2*sizeof(T) * (*(p_metricSO[p_subObjNum].pointsCount[i])) + 2*sizeof(short);
	}
	p_subObjNum++;		// Увеличиваем счетчик подобъектов
}

// template <class T1, class T2>
// void cMap::findSOmetric3d(unsigned short &p_metricNum, unsigned short &p_subObjNum, sМetric_3d<T1,T2> *p_metric, sМetric_3d<T1,T2> *p_metricSO, unsigned long &p_recordNum, unsigned long &curPos)
// {
// 	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
// 	p_metricSO[p_subObjNum].recordNum = p_metric[p_metricNum].header;
// 	p_metricSO[p_subObjNum].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
// 	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
// 	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*(2*sizeof(T1) + sizeof(T2)) + sizeof(short);

// 	cout << "======Record #" << p_recordNum << "======\n";

// 	for (int j = 0; j < m_ptr_headers[p_recordNum]->subobjectCount; j++)
// 	{
// 		// Указатель на служебное поле, содержащее количество точек подобъекта:				
// 		p_metricSO[p_subObjNum].pointsCount[j] 	= reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		
// 		p_metricSO[j].p3d_points 				= reinterpret_cast<s3dPoint<T1, T2>*>	(m_ptr_charMap + subPosition + sizeof(short));

// 		cout << "subObject #" << j << "\n";
// 		cout << "Points count = " << *(p_metricSO[p_subObjNum].pointsCount[j]) << "\n";

// 		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
// 		subPosition += (2*sizeof(T1) + sizeof(T2)) * (*(p_metricSO[p_subObjNum].pointsCount[j])) + 2*sizeof(short); 
// 	}
// 	p_subObjNum++;		// Увеличиваем счетчик подобъектов
// }




/////////////////////////////////////////////////////////////////////
void cMap::locate3dShortMetric(unsigned short &p_metricNum, unsigned short &p_subObjNum, unsigned long &p_recordNum, unsigned long &curPos)
{
	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
	m_p3d_ShortSO[p_subObjNum].header = m_ptr_records_short_3d[p_metricNum].header;
	m_p3d_ShortSO[p_subObjNum].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*(2*sizeof(short) + sizeof(float)) + sizeof(short);

	for (int i = 0; i < m_ptr_headers[p_recordNum]->subobjectCount; i++)
	{
		// Указатель на служебное поле, содержащее количество точек подобъекта:				
		m_p3d_ShortSO[p_subObjNum].pointsCount[i] 	= reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		
		m_p3d_ShortSO[p_subObjNum].p3d_points[i] 				= reinterpret_cast<s3dPoint<short, float>*>	(m_ptr_charMap + subPosition + sizeof(short));

		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
		subPosition += (2*sizeof(short) + sizeof(float)) * (*(m_p3d_ShortSO[p_subObjNum].pointsCount[i])) + 2*sizeof(short); 
	}
	p_subObjNum++;		// Увеличиваем счетчик подобъектов
}
/////////////////////////////////////////////////////////////////////
void cMap::locate3dLongMetric(unsigned short &p_metricNum, unsigned short &p_subObjNum, unsigned long &p_recordNum, unsigned long &curPos)
{
	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
	m_p3d_LongSO[p_subObjNum].header = m_ptr_records_long_3d[p_metricNum].header;
	m_p3d_LongSO[p_subObjNum].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*(2*sizeof(long) + sizeof(float)) + sizeof(short);

	for (int i = 0; i < m_ptr_headers[p_recordNum]->subobjectCount; i++)
	{
		// Указатель на служебное поле, содержащее количество точек подобъекта:				
		m_p3d_LongSO[p_subObjNum].pointsCount[i] 	= reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		
		m_p3d_LongSO[p_subObjNum].p3d_points[i] 	= reinterpret_cast<s3dPoint<long, float>*>	(m_ptr_charMap + subPosition + sizeof(short));

		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
		subPosition += (2*sizeof(long) + sizeof(float)) * (*(m_p3d_LongSO[p_subObjNum].pointsCount[i])) + 2*sizeof(short); 
	}
	p_subObjNum++;		// Увеличиваем счетчик подобъектов
}
/////////////////////////////////////////////////////////////////////
void cMap::locate3dFloatMetric(unsigned short &p_metricNum, unsigned short &p_subObjNum, unsigned long &p_recordNum, unsigned long &curPos)
{
	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
	m_p3d_FloatSO[p_subObjNum].header = m_ptr_records_float_3d[p_metricNum].header;
	m_p3d_FloatSO[p_subObjNum].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*3*sizeof(float) + sizeof(short);

	for (int i = 0; i < m_ptr_headers[p_recordNum]->subobjectCount; i++)
	{
		// Указатель на служебное поле, содержащее количество точек подобъекта:				
		m_p3d_FloatSO[p_subObjNum].pointsCount[i] 	= reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		
		m_p3d_FloatSO[p_subObjNum].p3d_points[i] 	= reinterpret_cast<s3dPoint<float, float>*>	(m_ptr_charMap + subPosition + sizeof(short));


		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
		subPosition += 3*sizeof(float) * (*(m_p3d_FloatSO[p_subObjNum].pointsCount[i])) + 2*sizeof(short); 
	}
	p_subObjNum++;		// Увеличиваем счетчик подобъектов
}
/////////////////////////////////////////////////////////////////////
void cMap::locate3dDoubleMetric(unsigned short &p_metricNum, unsigned short &p_subObjNum, unsigned long &p_recordNum, unsigned long &curPos)
{
	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
	m_p3d_DoubleSO[p_subObjNum].header = m_ptr_records_double_3d[p_metricNum].header;
	m_p3d_DoubleSO[p_subObjNum].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*3*sizeof(double) + sizeof(short);

	cout << "======Record #" << p_recordNum << "======\n";

	for (int i = 0; i < m_ptr_headers[p_recordNum]->subobjectCount; i++)
	{
		// Указатель на служебное поле, содержащее количество точек подобъекта:				
		m_p3d_DoubleSO[p_subObjNum].pointsCount[i] 	= reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		

		m_p3d_DoubleSO[p_subObjNum].p3d_points[i] = reinterpret_cast<s3dPoint<double, double>*>	(m_ptr_charMap + subPosition + sizeof(short));

		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
		subPosition += 3*sizeof(double) * (*(m_p3d_DoubleSO[p_subObjNum].pointsCount[i])) + 2*sizeof(short); 
	}
	p_subObjNum++;		// Увеличиваем счетчик подобъектов
}
///////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
void cMap::setPointers()
{
	// Количество байт данных карты, которые уже были интерпретированы:
	unsigned long currentPosition = sizeof(sPassport) + sizeof(sDescriptor);

	// Итераторы для заполнения разных массивов метрик объектов:
	unsigned short iter_2dS = 0;
	unsigned short iter_2dL = 0;
	unsigned short iter_2dF = 0;
	unsigned short iter_2dD = 0;
	unsigned short iter_3dS = 0;
	unsigned short iter_3dL = 0;
	unsigned short iter_3dF = 0;
	unsigned short iter_3dD = 0;
	// Итераторы для заполнения разных массивов метрик подобъектов:
	unsigned short iter_2dS_SO = 0;
	unsigned short iter_2dL_SO = 0;
	unsigned short iter_2dF_SO = 0;
	unsigned short iter_2dD_SO = 0;
	unsigned short iter_3dS_SO = 0;
	unsigned short iter_3dL_SO = 0;
	unsigned short iter_3dF_SO = 0;
	unsigned short iter_3dD_SO = 0;

	// Устанавливаем указатели заголовков записей и их метрик
	for(unsigned long i = 0; i < m_ptr_descriptor->recordCount; i ++)	// Цикл по количеству записей в карте
	{
		if (m_ptr_headers[i]->flag_dimension == 0)				// Определение размерности метрики (2D/3D)
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Определение типа данных метрика (Целое/Плав.т.)
			{						
				if(m_ptr_headers[i]->flag_metricSize == 0)		// Определение длины типа данных (2-4 B/4-8 B)
				{
					m_ptr_records_short_2d[iter_2dS].header = m_ptr_headers[i];
					m_ptr_records_short_2d[iter_2dS].p2d_points = reinterpret_cast<s2dPoint<short>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						findSOmetric2d<short>(iter_2dS, iter_2dS_SO, m_ptr_records_short_2d, m_p2d_ShortSO, i, currentPosition);
					}
					iter_2dS++;
				}
				else											// long
				{
					m_ptr_records_long_2d[iter_2dL].header = m_ptr_headers[i];
					m_ptr_records_long_2d[iter_2dL].p2d_points = reinterpret_cast<s2dPoint<long>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						findSOmetric2d<long>(iter_2dL, iter_2dL_SO, m_ptr_records_long_2d, m_p2d_LongSO, i, currentPosition);
					}
					iter_2dL++;
				}
			}
			else 												// Формат плавающей точки
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float
				{
					m_ptr_records_float_2d[iter_2dF].header = m_ptr_headers[i];
					m_ptr_records_float_2d[iter_2dF].p2d_points = reinterpret_cast<s2dPoint<float>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));

					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						findSOmetric2d<float>(iter_2dF, iter_2dF_SO, m_ptr_records_float_2d, m_p2d_FloatSO, i, currentPosition);
					}
					else if (m_ptr_headers[i]->flag_metricText == 1)
					{
						// cout << "ARrrr\n"; 
					}
					iter_2dF++;
				}
				else 											// double
				{
					m_ptr_records_double_2d[iter_2dD].header = m_ptr_headers[i];
					m_ptr_records_double_2d[iter_2dD].p2d_points = reinterpret_cast<s2dPoint<double>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						findSOmetric2d<double>(iter_2dD, iter_2dD_SO, m_ptr_records_double_2d, m_p2d_DoubleSO, i, currentPosition);
					}
					iter_2dD++;
				}
			}
		}
		else 													// Метрика в 3D
		{
			if (m_ptr_headers[i]->flag_metricType == 0)			// Целый тип
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// short/float, где float - высота
				{
					m_ptr_records_short_3d[iter_3dS].header = m_ptr_headers[i];
					m_ptr_records_short_3d[iter_3dS].p3d_points = reinterpret_cast<s3dPoint<short, float>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						locate3dShortMetric(iter_3dS, iter_3dS_SO, i, currentPosition);
						// findSOmetric3d<short, float>(iter_3dS, iter_3dS_SO, m_ptr_records_short_3d, m_p3d_ShortSO, i, currentPosition);
					}
					iter_3dS++;
				}
				else 											// long/float
				{
					m_ptr_records_long_3d[iter_3dL].header = m_ptr_headers[i];
					m_ptr_records_long_3d[iter_3dL++].p3d_points = reinterpret_cast<s3dPoint<long, float>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						locate3dLongMetric(iter_3dL, iter_3dF_SO, i, currentPosition);
					}
					iter_3dL++;
				}
			}
			else 												// Плавающая точка
			{
				if(m_ptr_headers[i]->flag_metricSize == 0)		// float/float
				{
					m_ptr_records_float_3d[iter_3dF].header = m_ptr_headers[i];
					m_ptr_records_float_3d[iter_3dF++].p3d_points = reinterpret_cast<s3dPoint<float, float>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						locate3dFloatMetric(iter_3dF, iter_3dF_SO, i, currentPosition);
					}
					iter_3dF++;
				}
				else 											// double/double
				{
					m_ptr_records_double_3d[iter_3dD].header = m_ptr_headers[i];
					m_ptr_records_double_3d[iter_3dD++].p3d_points = reinterpret_cast<s3dPoint<double, double>*>(m_ptr_charMap + currentPosition + sizeof(sHeader));
					if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
					{
						locate3dDoubleMetric(iter_3dD, iter_3dS_SO, i, currentPosition);
					}
					iter_3dD++;
				}
			}
		}

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





/////////////////////////////////////////////////////////////////////
template<class T>
void cMap::writeTypeMetric2d(std::ofstream &p_outfile, short &p_typePointsCounts, sМetric_2d<T>* p_metric, short &p_typePointsSOCounts, sМetricSO_2d<T>* p_metricSO)
{
	for (int i = 0; i < p_typePointsCounts; i ++)			// Цикл по всем основным метрикам 
	{

		unsigned long pointsCount;
		if (p_metric[i].header->metricPointsCount != 65535)	// Проверяем число точек в метрике
			pointsCount = p_metric[i].header->metricPointsCount;
		else
			pointsCount = p_metric[i].header->metricPointsCountBig;

		p_outfile << "Metric #" << i << "\n";

		for (int j = 0; j < pointsCount; j++) 		// Записываем метрику
			p_outfile << p_metric[i].p2d_points[j].X <<
			  	" - " << p_metric[i].p2d_points[j].Y << "\n";

		if (p_metric[i].header->subobjectCount > 0)					// Если у записи есть подобъекты
		{

			// Ищем подобъекты, которые принадлежат этой записи
			short SONum;
			for (int j = 0; j < m_p2d_floatCountSO; j++)			// Цикл по всем подобъекам
			{
				if (p_metric[i].header == p_metricSO[j].header)
				{
					SONum = j;
					break;
				}
			}
			

			for (int j = 0; j < p_metric[i].header->subobjectCount; j++)	// Цикл по всем подобъектам
			{
				p_outfile << "SubMetric #" << j << "\n";
				for (int k = 0; k < *(p_metricSO[SONum].pointsCount[j]); k++)
				{
					p_outfile	<< p_metricSO[SONum].p2d_points[j][k].X << 
					" - " 	<< p_metricSO[SONum].p2d_points[j][k].Y << "\n";
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////
template<class T1, class T2, template<class, class> class C = sМetric_3d>
void cMap::writeTypeMetric3d(std::ofstream &p_outfile, short &p_typePointsCounts, C<T1, T2>* p_metric)
{
	for (int i = 0; i < p_typePointsCounts; i ++)	// Цикл по всем записями типа short 2В
	{
		unsigned long pointsCount;
		if (p_metric[i].header->metricPointsCount != 65535)
			pointsCount = p_metric[i].header->metricPointsCount;
		else
			pointsCount = p_metric[i].header->metricPointsCountBig;

			p_outfile << "Metric #" << i << "\n";
		for (int j = 0; j < pointsCount; j++)
			p_outfile << p_metric[i].p3d_points[j].X <<
			  " - " << p_metric[i].p3d_points[j].Y << 
			  " - " << p_metric[i].p3d_points[j].H << "\n";
	}
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

	// outfile << "======= 2D SHORT POINTS ========\n"; 
	// writeTypeMetric2d	<short>				(outfile, m_p2d_shortCount, m_ptr_records_short_2d, m_p2d_shortCountSO);
	// outfile << "======= 2D LONG POINTS ========\n";
	// writeTypeMetric2d	<long>				(outfile, m_p2d_longCount, m_ptr_records_long_2d, m_p2d_longCountSO);
	outfile << "======= 2D FLOAT POINTS ========\n";
	writeTypeMetric2d	<float>				(outfile, m_p2d_floatCount, m_ptr_records_float_2d, m_p2d_floatCountSO, m_p2d_FloatSO);
	// outfile << "======= 2D DOUBLE POINTS ========\n";
	// writeTypeMetric2d	<double>			(outfile, m_p2d_doubleCount, m_ptr_records_double_2d, m_p2d_doubleCountSO);
	// outfile << "======= 3D SHORT POINTS ========\n";
	// writeTypeMetric3d	<short, float>		(outfile, m_p3d_shortCount, m_ptr_records_short_3d);
	// outfile << "======= 3D LONG POINTS ========\n";
	// writeTypeMetric3d	<long, float>		(outfile, m_p3d_floatCount, m_ptr_records_long_3d);
	// outfile << "======= 3D FLOAT POINTS ========\n";
	// writeTypeMetric3d	<float, float>		(outfile, m_p3d_floatCount, m_ptr_records_float_3d);
	// outfile << "======= 3D DOUBLE POINTS ========\n";
	// writeTypeMetric3d	<double, double>	(outfile, m_p3d_doubleCount, m_ptr_records_double_3d);

	cout << "Metric was recored in " << p_ptr_filename << "\n";
	outfile.close();
	return 1;
}
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
void cMap::showPCounts()
{
	cout 	<< m_p2d_shortCount		<< "\n"
			<< m_p2d_longCount 		<< "\n"
			<< m_p2d_floatCount		<< "\n"
			<< m_p2d_doubleCount	<< "\n"
			<< m_p3d_shortCount		<< "\n"
			<< m_p3d_longCount		<< "\n"
			<< m_p3d_floatCount		<< "\n"
			<< m_p3d_doubleCount	<< "\n\n"

			<< m_p2d_shortCountSO		<< "\n"
			<< m_p2d_longCountSO 		<< "\n"
			<< m_p2d_floatCountSO		<< "\n"
			<< m_p2d_doubleCountSO		<< "\n"
			<< m_p3d_shortCountSO		<< "\n"
			<< m_p3d_longCountSO		<< "\n"
			<< m_p3d_floatCountSO		<< "\n"
			<< m_p3d_doubleCountSO		<< "\n\n";
}
/////////////////////////////////////////////////////////////////////






















// /////////////////////////////////////////////////////////////////////
// template <class T, template<class> class C = s2dPoint>
// void cMap::locTemp(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos)
// {
// 	// Записываем к какому объекту относятся подобъекты и сколько каждого подобъекта точек:
// 	m_p2d_FloatSO[p_iteratorSO].recordNum = p_iterator;
// 	m_p2d_FloatSO[p_iteratorSO].pointsCount = new unsigned short*[m_ptr_headers[p_recordNum]->subobjectCount];
// 	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
// 	unsigned long subPosition = sizeof(sHeader) + curPos + m_ptr_headers[p_recordNum]->metricPointsCount*2*sizeof(T) + sizeof(short);

// 	cout << "======Record #" << p_recordNum << "======\n";

// 	for (int j = 0; j < m_ptr_headers[p_recordNum]->subobjectCount; j++)
// 	{
// 		// Указатель на служебное поле, содержащее количество точек подобъекта:				
// 		m_p2d_FloatSO[p_iteratorSO].pointsCount[j] 	= reinterpret_cast<unsigned short*>		(m_ptr_charMap + subPosition);		
// 		m_p2d_FloatSO[j].p2d_points 				= reinterpret_cast<C<T>*>	(m_ptr_charMap + subPosition + sizeof(short));

// 		cout << "subObject #" << j << "\n";
// 		cout << "Points count = " << *(m_p2d_ShortSO[p_iteratorSO].pointsCount[j]) << "\n";

// 		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
// 		subPosition += 2*sizeof(T) * (*(m_p2d_ShortSO[p_iteratorSO].pointsCount[j])) + 2*sizeof(short); 
// 	}
// 	p_iteratorSO++;		// Увеличиваем счетчик подобъектов
// }









		// if (m_ptr_headers[i]->subobjectCount > 0)	// Ищем подобъекты если они есть
		// {
		// 	cout << "======Record #" << i << "======\n";
			
		// 	// cout << "subObjectCount = " << m_ptr_records[i].header->subobjectCount << "\n";
		// 	// Количество байт от начала файла карты на которое нужно сместиться чтобы прочитать текущий подобъект:
		// 	unsigned long subPosition = sizeof(sHeader) + currentPosition + m_ptr_headers[i]->metricPointsCount*mType + sizeof(short); 
		// 	for (int j = 0; j < m_ptr_headers[i]->subobjectCount; j++)
		// 	{
		// 		cout << "subObject #" << j << "\n";
		// 		// указатель на служебное поле, содержащее количество точек подобъекта:				
		// 		unsigned short* pCount = reinterpret_cast<unsigned short*>
		// 		(	m_ptr_charMap +		// Указатель на массив с бинарной картой
		// 			currentPosition + 	// Количество байт, которое уже было прочитано до текущей записи
		// 			sizeof(sHeader) + 	// Размер заголовка, с которого начиается запись 
		// 			m_ptr_headers[i]->metricPointsCount*mType +	// Размер метрики записи
		// 			sizeof(short) + 	// Служебное поле с резервом, которое находится после метрики
		// 			subPosition);		// Сумма размеров метрик уже пройденных подобъектов

		// 		subobjectPoints[j] = reinterpret_cast<unsigned short*>
		// 		(	m_ptr_charMap + 
		// 		 	currentPosition +
		// 		 	sizeof(sHeader) +
		// 		 	m_ptr_headers[i]->metricPointsCount*2*sizeof(float) +
		// 		 	2*sizeof(short) +	// Служебное поле с резервом + поле с количеством точек
		// 		 	subPosition);

		// 		cout << "Points count = " << *pCount << "\n";

		// 		// Увеличиваем сетчик на размер всех точек подобъекта + 2 служебных поля с данными о количестве точек
		// 		subPosition += mType * (*pCount) + 2*sizeof(short); 
		// 	}
		// }
		// Увеличваем счетчик интерпретированных байт на размер размер записи

















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