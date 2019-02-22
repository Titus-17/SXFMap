//Map.h
#ifndef MAP_H
#define MAP_H
#include "Structs.h"
#include <fstream>

class cMap
{
private:
	char*			m_ptr_charMap;				// Массив для сырой бинарной  карты
	sPassport*		m_ptr_passport;				// Указатель на пасспорт в бинарной  карте
	sDescriptor* 	m_ptr_descriptor;			// Указатель на дескриптор в бинарной  карте
	sHeader**		m_ptr_headers;
	// Количество метрик объектов каждого типа
	short 			m_p2d_shortCount;			// Двумерные точки
	short 			m_p2d_longCount;
	short 			m_p2d_floatCount;
	short 			m_p2d_doubleCount;
	short 			m_p3d_shortCount;			// Трехмерные точки
	short 			m_p3d_longCount;
	short 			m_p3d_floatCount;
	short 			m_p3d_doubleCount;
	// Количество метрик подобъектов каждого типа
	short 			m_p2d_shortCountSO;			// Двумерные точки
	short 			m_p2d_longCountSO;
	short 			m_p2d_floatCountSO;
	short 			m_p2d_doubleCountSO;
	short 			m_p3d_shortCountSO;			// Трехмерные точки
	short 			m_p3d_longCountSO;
	short 			m_p3d_floatCountSO;
	short 			m_p3d_doubleCountSO;
	// Массивы указателей на метрики
	sМetric_2d	<short>*			m_ptr_records_short_2d;		// Двумерные точки
	sМetric_2d	<long>*				m_ptr_records_long_2d;
	sМetric_2d	<float>*			m_ptr_records_float_2d;
	sМetric_2d	<double>*			m_ptr_records_double_2d;
	sMetric_3d	<short, float>*		m_ptr_records_short_3d;		// Трехмерные точки
	sMetric_3d	<long, float>*		m_ptr_records_long_3d;
	sMetric_3d	<float, float>*		m_ptr_records_float_3d;
	sMetric_3d	<double, double>*	m_ptr_records_double_3d;
	// Массивы массивов указателей на метрики подобъектов
	sМetricSO_2d	<short>*		m_p2d_ShortSO;
	sМetricSO_2d	<long>* 		m_p2d_LongSO;
	sМetricSO_2d	<float>*		m_p2d_FloatSO;
	sМetricSO_2d	<double>*		m_p2d_DoubleSO;
	sMetricSO_3d	<short, float>* m_p3d_ShortSO;
	sMetricSO_3d	<long, float>*	m_p3d_LongSO;
	sMetricSO_3d	<float, float>*	m_p3d_FloatSO;
	sMetricSO_3d	<double, double>*m_p3d_DoubleSO;
public:
	cMap();
	~cMap();
	// Прочитать карту в битовом режиме и сохранить в m_ptr_charMap 
	short readSXF(char* p_filename); 

	void calcPointsCount();	// Посчитать количество метрик разных типов и записать в m_p2d_(...)Count
	void setPointers();		// Установить указатели на объекты в бинарной  карте 
	void AllocMem4Points();	// Выделить память под m_p2d_(...)Count указатели на точки 

	short writePassportLog(char* p_ptr_filename);		// Записать лог данных паспорта в файл
	short writeDescriptorLog(char* p_ptr_filename);		// Записать лог данных дескриптора в файл
	short writeHeadersLog(char* p_ptr_filename);		// Записать лог данных заголовков в файл
	short writeRecordMetricLog(char* p_ptr_filename);	// Записать лог данных метрик в файл
	short getPointsSize(long i);	 					// Определить размер точки (в байтах)
	void showPCounts();

	template<class T>
	void writeTypeMetric2d(std::ofstream &outfile, short &p_typePointsCounts, sМetric_2d<T>* p_metric, short &p_typePointsSOCounts, sМetricSO_2d<T>* p_metricSO);

	template<class T1, class T2, template<class, class> class C = sМetric_3d>
	void writeTypeMetric3d(std::ofstream &p_outfile, short &p_typePointsCounts, C<T1, T2>* p_metric);

	// void locate2dShortMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);
	// void locate2dLongMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);
	// void locate2dFloatMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);
	// void locate2dDoubleMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);

	void locate3dShortMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);
	void locate3dLongMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);
	void locate3dFloatMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);
	void locate3dDoubleMetric(unsigned short &p_iterator, unsigned short &p_iteratorSO, unsigned long &p_recordNum, unsigned long &curPos);

	template <class T>
	void findSOmetric2d(unsigned short &p_metricNum, unsigned short &p_subObjNum, sМetric_2d<T> *p_metric, sМetricSO_2d<T> *p_metricSO, unsigned long &p_recordNum, unsigned long &curPos);
	// template <class T1, class T2>
	// void findSOmetric3d(unsigned short &p_metricNum, unsigned short &p_subObjNum, sМetric_3d<T1,T2> *p_metric, sМetric_3d<T1,T2> *p_metricSO, unsigned long &p_recordNum, unsigned long &curPos);
};
#endif