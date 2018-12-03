//main.cpp
#include "Map.h"
#include <cstdlib> // system()

int main()
{
	cMap Map;
	Map.readPassport("M3833.SXF");
	system("pause");
	return 0;
}