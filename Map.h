//Map.h
#ifndef MAP_H
#define MAP_H
#include "Structs.h"
#include <fstream>

class cMap
{
private:
	char*			m_ptr_charMap;		// Массив для сырой битовой карты
	sPassport*		m_ptr_passport;		// Указатель на пасспорт в битовой карте
	sDescriptor* 	m_ptr_descriptor;	// Указатель на дескриптор в битовой карте
	sRecord*		m_ptr_records;		// Массив записей
public:
	cMap();
	~cMap();
	// Прочитать карту в битовом режиме и сохранить в m_ptr_charMap 
	short readSXF(char* p_filename); 

	void setPointers();					// Установить указатели на объекты в битовой карте 
	// Прочитать одну запись. p_recordNo - номер записи, p_bufPosition байт, после которого записана карта
	void readOneRecordMetric(long &p_recordNo, unsigned long &p_bufPosition);

	short writePassportLog(char* p_ptr_filename);		// Записать лог данных паспорта в файл
	short writeDescriptorLog(char* p_ptr_filename);		// Записать лог данных дескриптора в файл
	short writeHeadersLog(char* p_ptr_filename);		// Записать лог данных заголовков в файл
	short writeRecordMetricLog(char* p_ptr_filename);	// Записать лог данных метрик в файл
};

#endif