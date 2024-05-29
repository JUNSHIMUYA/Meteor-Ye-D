
#include "MSSFunctionalities.h"


void MSSrunTest(string str, string whichTest, string &network)
{
	if (str.compare("Debug") == 0)
	{
		cout << "debug deleted" << endl;
	}
	else if (str.compare("Test") == 0)
	{
		
		if(whichTest.compare("MSSMUTL") == 0){
			network = "Test MSS MUTL";
			testMSSMUTL(1, 1);
		}
		else if(whichTest.compare("MSSMatMul") == 0){
			network = "Test MSS MatMul";
			testMSSMatMul(1, 4, 1, 3);
		}
		else if(whichTest.compare("MSSComp") == 0){
			network = "Test MSS Comp";
			testComp(1, 1);
		}
		else
			assert(false && "Unknown test mode selected");
	}
	else
		assert(false && "Only Debug or Test mode supported");
}