#include <iostream>
#include <string>
#include "AESObject.h"
#include "Precompute.h"
#include "secondary.h"
#include "connect.h"
#include "./PDTE/MSSunitTests.h"
#include "./PDTE/dectree.h"
#include "./PDTE/FeaSel.h"
#include "./PDTE/Comp.h"
#include "./PDTE/SelChild.h"
#include "./PDTE/NextNode.h"
#include "tools.h"
#include <chrono>

//ASS Tree

int partyNum;
AESObject* aes_indep;
AESObject* aes_next;
AESObject* aes_prev;
Precompute PrecomputeObject;


int main(int argc, char** argv)
{
/****************************** PREPROCESSING ******************************/ 
	parseInputs(argc, argv);

	string operation, dectree_dir, dectree_filename, security;
	bool PRELOADING = true;
	aes_indep = new AESObject(argv[3]);
	aes_next = new AESObject(argv[4]);
/****************************** SELECT DECTREE FILENAME ******************************/ 
	if (argc == 8)
	{operation = argv[6]; dectree_dir = argv[7]; security = argv[8];}
	else
	{
		operation = "Test for MSS";
		dectree_dir = "/mycode/PDTE1/files/UCI_dectrees/";
		security = "Semi-honest";
	}

/********************************** Tree Params **************************************/

	dectree_filename = "mnist";
	
	DecTree tree;
	tree.read_from_file(dectree_dir + dectree_filename);
	cout << tree.num_attributes<<endl;
	int dim = tree.num_attributes;
	if(dectree_filename == "mnist" ) dim = 784;
	tree.depthPad(); //
	vector<uint64_t>featureVec;
	for(int i = 0; i < dim; i++) 
	{
		featureVec.push_back(rand());
	}
	MSSVectorType MSSfeavec = fecVec(featureVec);
	
	ASSDecTree ASStree;
	ASStree.DecTree2ASSDecTree(tree);

	int I = tree.node_vec[0]->attribute_index;//fix random position of the 0 vec;
	ASSVectorMyType randomVec(dim*1, 0);
	randomVec[I] = 1; 

	
	
/****************************** AES SETUP and SYNC ******************************/ 
	
	initializeCommunication(argv[2], partyNum);
	synchronize(2000000);
	cout << "connected success" << endl;


	start_m(); 
	
	//MSSrunTest("Test", "MSSMUTL", operation);

	MSSVectorType FeaSelR = FeaSel(MSSfeavec, randomVec, 1, dim, 1);

	MSSType CompR = Comp(FeaSelR[0], ASStree.node_vec[0]->threshold);
	
	myType Tid = SelCh(ASStree.node_vec[0]->right_child_id, ASStree.node_vec[0]->left_child_id, CompR);
	
	ASSVectorMyType GetNextNodeR = GetNextNode(Tid, ASStree, 0);
	
	cout<<"----------------------------------------------"<<endl;
	cout<<"                  "<<"Layer:0"<<"             "<<endl;
	cout<<"----------------------------------------------"<<endl;
	cout<<"CurrentId:   "<<"0"<<endl;
	cout<<"ASStreeLchild: "<<ASStree.node_vec[0]->left_child_id<<" ASStreeRchild: "<<ASStree.node_vec[0]->right_child_id<<endl;
	cout<<"threshold: "<<ASStree.node_vec[0]->threshold<<endl;
	cout<<"Feature:   "<<FeaSelR[0].first<<endl;
	cout<<"Comp:   "<<CompR.first<<endl;
	cout<<"GetNextNodeId: "<< Tid <<endl;

	randomVec[I] = 0; 
	
	int tem;
	for(int i = 1; i < tree.depth; i++) //depth
	{  
		tem = GetNextNodeR[0];
		randomVec[tem] = 1; 

		cout<<"----------------------------------------------"<<endl;
		cout<<"                 "<<"Layer:"<<i<<"            "<<endl;
		cout<<"----------------------------------------------"<<endl;
		cout<<"CurrentId:   "<<Tid<<endl;
		cout<<"ASStreeLchild: "<<GetNextNodeR[2]<<" ASStreeRchild: "<<GetNextNodeR[3]<<endl;
		cout<<"threshold: "<<GetNextNodeR[1]<<endl;
		
		FeaSelR = FeaSel(MSSfeavec, randomVec, 1, dim, 1);
		
		CompR = Comp(FeaSelR[0], GetNextNodeR[1]);
		
		Tid = SelCh(GetNextNodeR[3], GetNextNodeR[2], CompR);

		cout<<"Feature:   "<<FeaSelR[0].first<<endl;
		cout<<"Comp:   "<<CompR.first<<endl;
		cout<<"GetNextNodeId: "<< Tid <<endl;

		GetNextNodeR = GetNextNode(Tid, ASStree, 0);
		
		randomVec[tem] = 0; 
	}

		cout<<"----------------------------------------------"<<endl;
		cout<<"                  "<<"Result"<<"              "<<endl;
		cout<<"----------------------------------------------"<<endl;
		cout<<"Get the classification: "<<ASStree.node_vec[Tid + 0]->classification<<endl;

	end_m(operation);
	
	cout << "----------------------------------------------" << endl;  	
	cout << "Run details: " << NUM_OF_PARTIES << "PC (P" << partyNum 
			<< "), " << NUM_ITERATIONS << "Running " << security << " " << operation << " on " << dectree_filename << " dectree" << endl;
	cout << "----------------------------------------------" << endl << endl;  
	


/****************************** CLEAN-UP ******************************/ 
	delete aes_indep;
	delete aes_next;
	

	return 0;
}




