//====================================Dijsktra Shortest Path====================================================
//Calculate the shortest path from the src node to all the other nodes

#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct UndirectedGraphNode {
      unsigned int label;   
      struct UndirectedGraphNode *neighbors[NEIGHBORS_MAX_SIZE];
      unsigned int neighborsCount;
};

/*
  This function calculates the Dijsktra's shortest path from the source node to all other nodes
  Input: graphMap[] - graph topology
         src - source node
         A[] - an array of shortest distrances from the source node to all other nodes 
 */ 
void DijsktraShortestPath(struct UndirectedGraphVertex *graphMap[], int src, int A[])
{
	int graphMapIdx = src-1;
	bool X[NUM_VERTICES];   
	int i,j;
	
	for (i = 0; i < NUM_VERTICES; i++)
	{
		X[i] = false;
	}

	X[graphMapIdx] = true;    
	A[graphMapIdx] = 0;

	while (true)
	{
		int shortestD, candidate; 
		bool first_time=true;
		for (i = 0; i < NUM_VERTICES; i++)//all the crossing edges from X to V-X 
		{
                       if (X[i] == true) 
		       {
			  for (j = 0; j<graphMap[i]->neighborsCount; j++)
			  {
				  if (X[graphMap[i]->neighbors_list[j]-1] == false) //this node in X has the crossing edge to V-X
				  {
					  if (first_time)
					  {
						shortestD = A[i] + graphMap[i]->neighbors_cost[j];
						candidate = graphMap[i]->neighbors_list[j];
						first_time = false;
					  }
					  else 
					  {
						int dist = A[i] + graphMap[i]->neighbors_cost[j];
						if (dist < shortestD)
						{
							shortestD = dist;
							candidate = graphMap[i]->neighbors_list[j];
						}
					  }
				  }
			 }
		     }
		}
		 
		if (first_time) //no new node is added to X
			break;

		//Add the node that minimizes A[v]+l_vw to X
		graphMapIdx = candidate - 1;
		X[graphMapIdx] = true; 
		A[graphMapIdx] = shortestD;

	} //while(true)
}

void TestShortestPath()
{

	FILE *fp;
	
	fp = fopen("dijkstraData.txt","r");

	char *pch;
	const int line_size = 500;
	char line[line_size];
	struct UndirectedGraphVertex *graphMap[NUM_VERTICES];
	int data,n;

    	if (fp != NULL){
		int graphMap_idx = 0;
		while (fgets(line, line_size, fp) != NULL)
		{
			struct UndirectedGraphVertex *node = (UndirectedGraphVertex *)malloc(sizeof(struct UndirectedGraphVertex));
			node->neighborsCount = 0;
			n = 0;
			pch = strtok (line," ,\t");   //for each line, read the number with \t as the delimiter
            		while (pch != NULL)
            		{
				data = atoi(pch);

				if (data == 0) break;
				if (n == 0)
				{
					node->label = data;
				}
				else
				{
					if ((n % 2) != 0)             //sink node
					{
						node->neighbors_list[node->neighborsCount] = data;
						node->neighborsCount++;
					}
					else						  //path cost
					{
						node->neighbors_cost[node->neighborsCount-1] = data;
					}
				}
				n++;
				pch = strtok (NULL," ,\t");
			  
			}
			graphMap[graphMap_idx] = node;
			graphMap_idx++;
		}
        	fclose (fp);
    	}

	int src_node = 1;
	int shortestDistArray[NUM_VERTICES];
	DijsktraShortestPath(graphMap,src_node,shortestDistArray);
	int display_node[10] = {7,37,59,82,99,115,133,165,188,197};

	for(int i = 0; i < 10; i++)
	{
		printf("Shortest Dist from node %d to node %d is %d\n",src_node,display_node[i],shortestDistArray[display_node[i]-1]);
	}
	
}
