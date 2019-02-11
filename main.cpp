//main.cpp
#include <iostream>
#include "Map.h"
#include <ctime>
#include <cstdlib> // system()

int main()
{

	unsigned int start_time =  clock(); // начальное время
    // здесь должен быть фрагмент кода, время выполнения которого нужно измерить
	cMap Map;
	if(!Map.readSXF("M3833.SXF"))
		return 1;
	Map.calcPointsCount();
	Map.AllocMem4Points();
	Map.showPCounts();
	Map.setPointers();

	Map.writePassportLog("PassportLog.txt");
	Map.writeDescriptorLog("DescriptorLog.txt");
	Map.writeHeadersLog("HeadersLog.txt");
	Map.writeRecordMetricLog("MetricLog.txt");
	// system("pause");
	Map.~Map();
    unsigned int end_time = clock(); // конечное время
    std::cout << "Time = " << end_time - start_time << "\n"; // искомое время
	return 0;
}