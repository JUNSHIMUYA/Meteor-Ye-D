

#ifndef DECTREE_H_INCLUDED
#define DECTREE_H_INCLUDED
#pragma once

#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <queue>
#include <math.h>       /* pow */
#include "../globals.h"


using namespace std;

class DecTree {
  public:
      struct Node {
        int id;
        // Pointer to the first child node
        Node* left;
        // Pointer to the second child node
        Node* right;
        // Pointer to the first parent node
        Node* parent;
        // True if the edges coming into the node are already colored
        uint32_t level;
        // True if the node is a leaf
        bool leaf;
        // Threshold in decision node to compare with
        uint64_t threshold;
        // Threshold in decision node to compare with
        uint64_t classification;
        // Attribute index to compare with, -1 if undefined
        uint32_t attribute_index;

        Node();
        Node(const Node&);
      };
   // Vector of nodes that belong to the Gamma1 graph
   vector<Node*> node_vec;
   // Vector of decision nodes that belong to the Gamma1 graph
   vector<Node*> decnode_vec;
   //Node** node_array;
   // Vector of all attributes used for comparison with their occurrence
   vector<uint64_t> attributes;
   // Vector of all thresholds used for comparison with their occurrence
   vector<uint64_t> thresholds;

   //STATISTICS FOR DECISION TREES
   //number of attributes
   uint32_t num_attributes;
   //number of decision nodes
   uint32_t num_dec_nodes;
   //depth = largest level of leaves
   uint32_t depth;
   //number of leaves
   uint32_t num_of_leaves;
   //dummy in our case
   uint32_t dummy_non_full;
   


   DecTree();
   DecTree(const DecTree&);

   void add_node(Node*);
   void add_decnode(Node*);
   void add_edge(Node*, Node*);
   void read_from_file(string);
   void evaluate(vector<uint64_t> inputs);
   void depthPad();
   void fullTree(uint32_t num_att, uint32_t depth);

   ~DecTree();
};

void tokenize(const std::string&, std::vector<string>&);
void eraseSubStr(std::string &, const std::string &);

//MSS

class ASSDecTree {
  public:
      struct ASSNode
      {
          int id;
              // Pointer to the first child node
                // True if the node is a leaf
          bool leaf;
          myType threshold;
          // Threshold in decision node to compare with
          myType classification;
          // Attribute index to compare with, -1 if undefined
          myType attribute_index;
          // leftchild Id
          myType left_child_id;
          // rightchild Id
          myType right_child_id;
      };
   // Vector of nodes that belong to the Gamma1 graph
   vector<ASSNode*> node_vec;
   // Vector of decision nodes that belong to the Gamma1 graph
   vector<ASSNode*> decnode_vec;
   //Node** node_array;
   // Vector of all attributes used for comparison with their occurrence
   vector<myType> attributes;
   // Vector of all thresholds used for comparison with their occurrence
   vector<myType> thresholds;

  void DecTree2ASSDecTree(DecTree& t);
   ~ASSDecTree();
};



vector<vector<DecTree::Node*>> levelOrder(DecTree::Node*root);
       



#endif // DECTREE_H_INCLUDED
