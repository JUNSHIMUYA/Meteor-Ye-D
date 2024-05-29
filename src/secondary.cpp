
#include "connect.h" 
#include "secondary.h"

extern CommunicationObject commObject;
extern int partyNum;
extern string * addrs;
extern BmrNet ** communicationSenders;
extern BmrNet ** communicationReceivers;
extern void log_print(string str);
#define NANOSECONDS_PER_SEC 1E9

//For time measurements
clock_t tStart;
struct timespec requestStart, requestEnd;
bool alreadyMeasuringTime = false;
int roundComplexitySend = 0;
int roundComplexityRecv = 0;
bool alreadyMeasuringRounds = false;

//For faster modular operations
extern smallType additionModPrime[PRIME_NUMBER][PRIME_NUMBER];
extern smallType subtractModPrime[PRIME_NUMBER][PRIME_NUMBER];
extern smallType multiplicationModPrime[PRIME_NUMBER][PRIME_NUMBER];


size_t trainDataBatchCounter = 0;
size_t trainLabelsBatchCounter = 0;
size_t testDataBatchCounter = 0;
size_t testLabelsBatchCounter = 0;

size_t INPUT_SIZE;
size_t LAST_LAYER_SIZE;
size_t NUM_LAYERS;
bool WITH_NORMALIZATION;
bool LARGE_NETWORK;
size_t TRAINING_DATA_SIZE;
size_t TEST_DATA_SIZE;
string SECURITY_TYPE;

extern void print_linear(myType var, string type);
extern void funcReconstruct(const ASSVectorMyType &a, vector<myType> &b, size_t size, string str, bool print);

/******************* Main train and test functions *******************/
void parseInputs(int argc, char* argv[])
{	
	if (argc < 5) 
		print_usage(argv[0]);

	partyNum = atoi(argv[1]);

	for (int i = 0; i < PRIME_NUMBER; ++i)
		for (int j = 0; j < PRIME_NUMBER; ++j)
		{
			additionModPrime[i][j] = ((i + j) % PRIME_NUMBER);
			subtractModPrime[i][j] = ((PRIME_NUMBER + i - j) % PRIME_NUMBER);
			multiplicationModPrime[i][j] = ((i * j) % PRIME_NUMBER); //How come you give the right answer multiplying in 8-bits??
		}
}


extern void print_vector(ASSVectorMyType &var, string type, string pre_text, int print_nos);
extern string which_network(string network);


// Generate a file with 0's of appropriate size
void generate_zeros(string name, size_t number, string network)
{
	string default_path = "files/preload/"+which_network(network)+"/";
	ofstream data_file;
	data_file.open(default_path+name);

	for (int i = 0; i < number; ++i)
		data_file << (int)0 << " ";
}


extern size_t nextParty(size_t party);


/********************* COMMUNICATION AND HELPERS *********************/

void start_m()
{
	// cout << endl;
	start_time();
	start_communication();
}

void end_m(string str)
{
	
	end_time(str);
	
	pause_communication();
	
	aggregateCommunication();
	
	end_communication(str);
}

void start_time()
{
	if (alreadyMeasuringTime)
	{
		cout << "Nested timing measurements" << endl;
		exit(-1);
	}

	tStart = clock();
	clock_gettime(CLOCK_REALTIME, &requestStart);
	alreadyMeasuringTime = true;
}

void end_time(string str)
{
	if (!alreadyMeasuringTime)
	{
		cout << "start_time() never called" << endl;
		exit(-1);
	}

	clock_gettime(CLOCK_REALTIME, &requestEnd);
	cout << "----------------------------------------------" << endl;
	cout << "Wall Clock time for " << str << ": " << diff(requestStart, requestEnd) << " sec\n";
	cout << "CPU time for " << str << ": " << (double)(clock() - tStart)/CLOCKS_PER_SEC << " sec\n";
	cout << "----------------------------------------------" << endl;	
	alreadyMeasuringTime = false;
}


void start_rounds()
{
	if (alreadyMeasuringRounds)
	{
		cout << "Nested round measurements" << endl;
		exit(-1);
	}

	roundComplexitySend = 0;
	roundComplexityRecv = 0;
	alreadyMeasuringRounds = true;
}

void end_rounds(string str)
{
	if (!alreadyMeasuringTime)
	{
		cout << "start_rounds() never called" << endl;
		exit(-1);
	}

	cout << "----------------------------------------------" << endl;
	cout << "Send Round Complexity of " << str << ": " << roundComplexitySend << endl;
	cout << "Recv Round Complexity of " << str << ": " << roundComplexityRecv << endl;
	cout << "----------------------------------------------" << endl;	
	alreadyMeasuringRounds = false;
}

void aggregateCommunication()
{
	vector<uint64_t> vec(4, 0), temp(4, 0);
	vec[0] = commObject.getSent();
	vec[1] = commObject.getRecv();
	vec[2] = commObject.getRoundsSent();
	vec[3] = commObject.getRoundsRecv();

	if (partyNum == PARTY_B )
		sendVector<uint64_t>(vec, PARTY_A, 4);

	if (partyNum == PARTY_A)
	{
		receiveVector<uint64_t>(temp, PARTY_B, 4);
		for (size_t i = 0; i < 4; ++i)
			vec[i] = temp[i] + vec[i];
	}
	
	if (partyNum == PARTY_A)
	{
		cout << "----------------------------------------------" << endl;
		cout << "Total communication: " << (float)vec[0]/1000000 << "MB (sent) and " << (float)vec[1]/1000000 << "MB (recv)\n";
		cout << "Total calls: " << vec[2] << " (sends) and " << vec[3] << " (recvs)" << endl;
		cout << "----------------------------------------------" << endl;
	}
}


void print_usage (const char * bin) 
{
    cout << "Usage: ./" << bin << " PARTY_NUM IP_ADDR_FILE AES_SEED_INDEP AES_SEED_NEXT AES_SEED_PREV" << endl;
    cout << endl;
    cout << "Required Arguments:\n";
    cout << "PARTY_NUM			Party Identifier (0,1, or 2)\n";
    cout << "IP_ADDR_FILE		\tIP Address file (use makefile for automation)\n";
    cout << "AES_SEED_INDEP		\tAES seed file independent\n";
    cout << "AES_SEED_NEXT		\t \tAES seed file next\n";
    cout << "AES_SEED_PREV		\t \tAES seed file previous\n";
    cout << endl;
    cout << "Report bugs to swagh@princeton.edu" << endl;
    exit(-1);
}

double diff(timespec start, timespec end)
{
    timespec temp;

    if ((end.tv_nsec-start.tv_nsec)<0)
    {
            temp.tv_sec = end.tv_sec-start.tv_sec-1;
            temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    }
    else 
    {
            temp.tv_sec = end.tv_sec-start.tv_sec;
            temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp.tv_sec + (double)temp.tv_nsec/NANOSECONDS_PER_SEC;
}


void deleteObjects()
{
	//close connection
	for (int i = 0; i < NUM_OF_PARTIES; i++)
	{
		if (i != partyNum)
		{
			delete communicationReceivers[i];
			delete communicationSenders[i];
		}
	}
	delete[] communicationReceivers;
	delete[] communicationSenders;
	delete[] addrs;
}




