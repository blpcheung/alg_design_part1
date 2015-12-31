//This program clones an undirected graph

#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NEIGHBORS_MAX_SIZE 2000

struct UndirectedGraphNode {
      unsigned int label;   
      struct UndirectedGraphNode *neighbors[NEIGHBORS_MAX_SIZE];
      unsigned int neighborsCount;
};

struct queue {
   struct UndirectedGraphNode *node[NEIGHBORS_MAX_SIZE];
   int top;
   int bottom;
};  

/*
 This program clones an undirected graph
 Input: graph - The pointer to the root node of the graph that is going to be cloned
 Output: the cloned graph 
*/
struct UndirectedGraphNode *cloneGraph(struct UndirectedGraphNode *graph) {
 
    if (graph == NULL) return NULL;
     
	struct queue q; 
   
    q.top = 0;
    q.bottom = 0;
   
    //Create a cloneMap whose indices are the same as graph label
    struct UndirectedGraphNode *cloneMap[NEIGHBORS_MAX_SIZE];
    int i;
    for (i = 0; i < NEIGHBORS_MAX_SIZE; i++)
    {
        cloneMap[i] = NULL;
    }
    
    //using queue for BFS
    q.node[q.bottom++] = graph;    //push the node into the queue
    
    //clone the root
    struct UndirectedGraphNode *cloneRoot = (UndirectedGraphNode *)malloc(sizeof(struct UndirectedGraphNode));
    cloneRoot->label = graph->label;
    cloneRoot->neighborsCount = graph->neighborsCount;

	//Assign the root to the cloneMap
    cloneMap[graph->label] = cloneRoot;
    
    while (q.bottom != q.top)
    {
        struct UndirectedGraphNode *n = q.node[(q.top++)%NEIGHBORS_MAX_SIZE];  //pop the queue
        //for each neighbor node of node n
        for (int i = 0; i < (int)n->neighborsCount; i++)
        {
            struct UndirectedGraphNode *neighbor = n->neighbors[i];    //get the neighbor node data structure
            //if not in the cloneMap
            if (cloneMap[neighbor->label] == NULL)
            {
                //clone a node
                struct UndirectedGraphNode *cloneNode = (UndirectedGraphNode *)malloc(sizeof(struct UndirectedGraphNode));
                cloneNode->label = neighbor->label;
                cloneNode->neighborsCount = neighbor->neighborsCount;
                
				//update cloneMap
                cloneMap[n->label]->neighbors[i] = cloneNode;  //assign the new node to the node n's neighbor list in the cloneMap;
                cloneMap[neighbor->label] = cloneNode;   //assign the new node to the cloneMap
                
                //Push the neighbor node into the queue
                q.node[q.bottom++%NEIGHBORS_MAX_SIZE] = neighbor;  
            }
            else
            {
		//update the n node's neighbor list in the cloneMap
                cloneMap[n->label]->neighbors[i]=cloneMap[neighbor->label];
            }
        }
    }
    return cloneRoot;
}

void TestCloneGraph()
{
/*
Create this graph for testing 
	   1
      / \
     /   \
    0 --- 2
         / \
         \_/
*/
	struct UndirectedGraphNode *graph = (UndirectedGraphNode *)malloc(sizeof(struct UndirectedGraphNode));
	struct UndirectedGraphNode *node0 = (UndirectedGraphNode *)malloc(sizeof(struct UndirectedGraphNode));
	struct UndirectedGraphNode *node1 = (UndirectedGraphNode *)malloc(sizeof(struct UndirectedGraphNode));
	struct UndirectedGraphNode *node2 = (UndirectedGraphNode *)malloc(sizeof(struct UndirectedGraphNode));

	node0->label = 0;
	node0->neighbors[0] = node1;
	node0->neighbors[1] = node2;
	node0->neighborsCount = 2;

	node1->label = 1;
	node1->neighbors[0] = node0;
	node1->neighbors[1] = node2;
	node1->neighborsCount = 2;

	node2->label = 2;
	node2->neighbors[0] = node0;
	node2->neighbors[1] = node1;
	node2->neighbors[2] = node2;
	node2->neighborsCount = 3;

	graph = node0;

	printf("graph->label=%d\n",graph->label);
	printf("graph->neighborsCount=%d\n",graph->neighborsCount);
	
	int j;
	for (j = 0; j < 2; j++)
	{
		printf("graph->neighbors[%d]->label=%d\n",j,graph->neighbors[j]->label);
	}

	struct UndirectedGraphNode *clone_graph = cloneGraph(graph);

	printf("clone_graph->label=%d\n",clone_graph->label);
	printf("clone_graph->neighborsCount=%d\n",clone_graph->neighborsCount);
	for (j = 0; j < 2; j++)
	{
		printf("clone_graph->neighbors[%d]->label=%d\n",j,clone_graph->neighbors[j]->label);
	}
	printf("node2->label = %d\n",clone_graph->neighbors[0]->neighbors[1]->label);
	printf("node1->label = %d\n",clone_graph->neighbors[0]->neighbors[1]->neighbors[1]->label);
	
}
