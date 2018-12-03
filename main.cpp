//main.cpp
#include "Map.h"
#include <cstdlib> // system()

int main()
{
	cMap Map;
	Map.readPassport("M3833.SXF");
	Map.writePassportLog("PassportLog.txt");
	system("pause");
	return 0;
}