#include "NextNode.h"
#include <thread>

ASSVectorMyType GetNextNode(myType& id, ASSDecTree& tree, uint64_t I)
{

    ASSVectorMyType res(4, 0);
    vector<myType>tid(1, id + 0); //id + I ,but we fix I = 0;

    vector<myType>revtid(1, 0);
    thread *threads = new thread[2];
	
    if(partyNum == 0)
    {
        threads[0] = thread(sendVector<myType>, ref(tid), nextParty(partyNum), 1);
        threads[1] = thread(receiveVector<myType>, ref(revtid), nextParty(partyNum), 1);
    }
    else
    {
        threads[0] = thread(sendVector<myType>, ref(tid), nextParty(partyNum), 1);
        threads[1] = thread(receiveVector<myType>, ref(revtid), nextParty(partyNum), 1);
    }

    for (int i = 0; i < 2; i++)
		threads[i].join();
		

    tid[0] = revtid[0] + 0 + tid[0]; 
    vector<myType>Delta(4, 0);

    if(partyNum == 0)
    {
        Delta[0] = tree.node_vec[tid[0]]->attribute_index;
        Delta[1] = tree.node_vec[tid[0]]->threshold;
        Delta[2] = tree.node_vec[tid[0]]->left_child_id;
        Delta[3] = tree.node_vec[tid[0]]->right_child_id;
    }
    else
    {
        Delta[0] = tree.node_vec[tid[0]]->attribute_index;
        Delta[1] = 0;
        Delta[2] = 0;
        Delta[3] = 0;
    }
   

    return Delta;
}