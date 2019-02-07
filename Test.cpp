//Test.cpp
#include <iostream>
using namespace std;

enum 	metricTypes {	mType_2dShort, mType_2dLong, mType_2dFloat, mType_2dDouble,
 						mType_3dShort, mType_3dLong, mType_3dFloat, mType_3dDouble};

int main()
{
	metricTypes Type = mType_3dDouble;
	cout << Type << "\n";
}    