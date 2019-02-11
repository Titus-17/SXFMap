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
	// Количество метрик каждого типа
	short 			m_p2d_ShortCount;			// Двумерные точки
	short 			m_p2d_LongCount;
	short 			m_p2d_FloatCount;
	short 			m_p2d_DoubleCount;
	short 			m_p3d_ShortCount;			// Трехмерные точки
	short 			m_p3d_LongCount;
	short 			m_p3d_FloatCount;
	short 			m_p3d_DoubleCount;
	// Массивы указателей на метрики
	s2dPoint<short>**			m_p2d_Short;	// Двумерные точки
	s2dPoint<long>**			m_p2d_Long;
	s2dPoint<float>**			m_p2d_Float;
	s2dPoint<double>**			m_p2d_Double;
	s3dPoint<short, float>**	m_p3d_Short;	// Трехмерные точки
	s3dPoint<long, float>**		m_p3d_Long;
	s3dPoint<float, float>**	m_p3d_Float;
	s3dPoint<double, double>**	m_p3d_Double;
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
};
#endif