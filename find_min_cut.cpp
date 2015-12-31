#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct UndirectedGraphVertex {
      unsigned int label;        //min. label is 1, not 0
      unsigned int neighbors_list[NEIGHBORS_MAX_SIZE];
	  unsigned int neighbors_cost[NEIGHBORS_MAX_SIZE];
      unsigned int neighborsCount;
};

#define NUM_VERTICES 200

int RandomContractAlg(struct UndirectedGraphVertex **graphMap)
{
	int i, num_existed_vertices=0, existed_vertices[NUM_VERTICES];

	//Count number of existed nodes
	for (i = 0; i < NUM_VERTICES; i++)
	{
		if (graphMap[i] != NULL)     
		{
			existed_vertices[num_existed_vertices++] = graphMap[i]->label;
		}
	}

	if (num_existed_vertices == 2)  //Only 2 nodes left.  We are done
	{
		return num_existed_vertices;
	}
	else
	{	
		//Randomly select the node
		int rand_num = rand();
		int rand_node = (int)floor((float)(rand_num)/(RAND_MAX+1) * num_existed_vertices);  //num_existed_vertices = [1...200] rand_node = [0...num_existed_vertices - 1]
	   
		struct UndirectedGraphVertex* merged_node = graphMap[existed_vertices[rand_node]-1];
	    
		//Randomly select the edge
		rand_num = rand();
		int rand_edge = (int)floor((float)(rand_num)/(RAND_MAX+1) * merged_node->neighborsCount);  //rand_edge = [0...num_existed_vertices - 1]
		
		//Merge two nodes
		int deleted_node_label = merged_node->neighbors_list[rand_edge];
		struct UndirectedGraphVertex* deleted_node = graphMap[deleted_node_label-1];   //Note: graphMap idx starts at 0
		int merged_node_label = merged_node->label;
	
		//Create a local array to faciliate the merge
		int k=0; 
		int *aux_neighbor_list = (int *)malloc(sizeof(int)*NEIGHBORS_MAX_SIZE);
		for (i = 0; i < merged_node->neighborsCount; i++)
		{
			if (merged_node->neighbors_list[i] != deleted_node_label)
				aux_neighbor_list[k++] = merged_node->neighbors_list[i]; 
		}
	
		for (i = 0; i < deleted_node->neighborsCount; i++)
		{
			if (deleted_node->neighbors_list[i] != merged_node_label)
			{
				aux_neighbor_list[k++] = deleted_node->neighbors_list[i]; 

				if (k >= NEIGHBORS_MAX_SIZE) 
				{	
					printf("Aux matrix access exceeds MAX_SIZE, Count=%d \n",merged_node->neighborsCount+deleted_node->neighborsCount); 
					return -1;
				}
				//Update the neighbor nodes of the deleted node to point to the merged node
				struct UndirectedGraphVertex* neighbor_node = graphMap[deleted_node->neighbors_list[i]-1];
				if (neighbor_node != NULL)
				{
					int j=0;
					while (neighbor_node->neighbors_list[j] != deleted_node_label)
					{
						j++;
						if (j >= neighbor_node->neighborsCount) 
						{
							printf("Neighbors_list access exceeds neighborsCount\n"); 
							return -1;
						}
					}
					neighbor_node->neighbors_list[j] = merged_node_label;
				}
			}
		}
	
		merged_node->neighborsCount = k;
	
		for (i = 0; i < merged_node->neighborsCount; i++)
		{
			merged_node->neighbors_list[i] = aux_neighbor_list[i];
		}
	
		free(aux_neighbor_list);

		//Set the deleted_node to NULL in the graphMap
		if (graphMap[deleted_node_label-1] != NULL)
		{
			graphMap[deleted_node_label-1] = NULL;
		}
		else
		{
			printf("Error, node %d has already been merged\n",deleted_node_label);
			return -1;
		}
		
		return num_existed_vertices-1;
	}

}

void TestFindMinCut()
{
	/*
	 1. Populate the UndirectedGraphNode structure for each line
	 2. Perform merge
	 3. Repeat until only 2 nodes left
	 */

	FILE *fp;
	
	fp = fopen("kargerMinCut.txt","r");

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
			n = 0;
			pch = strtok (line,"\t");   //for each line, read the number with \t as the delimiter
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
					node->neighbors_list[n-1] = data;
				}
				n++;
			    pch = strtok (NULL,"\t");
			}
			
			node->neighborsCount = n-1;
			graphMap[graphMap_idx] = node;
			graphMap_idx++;

		}
        fclose (fp);
    }

	int i,j,k,num_existed_nodes = 0;

	int N = NUM_VERTICES;
	int min_cut = INT_MAX;
	struct UndirectedGraphVertex *localGraphMap[NUM_VERTICES];

	srand(time(NULL));

	for (i = 0; i < N; i++)
	{
		//Create another copy of graph
		for (j = 0; j < NUM_VERTICES; j++)
		{
		  struct UndirectedGraphVertex *node = (UndirectedGraphVertex *)malloc(sizeof(struct UndirectedGraphVertex));
		  node->label = graphMap[j]->label;
		  node->neighborsCount = graphMap[j]->neighborsCount;
		  for (k = 0; k < node->neighborsCount; k++)
		  {
			node->neighbors_list[k] = graphMap[j]->neighbors_list[k];
		  }
		  localGraphMap[j] = node;
		}

		num_existed_nodes = RandomContractAlg(localGraphMap);
		while (num_existed_nodes > 2)
		{
			num_existed_nodes = RandomContractAlg(localGraphMap);
		}
	
		if (num_existed_nodes < 0)
		{
	       	printf("Error!!");
			return;
		}
	
		int remaining_node_idx = 0;
		while (localGraphMap[remaining_node_idx] == NULL)
			remaining_node_idx++;
		
		int min_cut_curr = localGraphMap[remaining_node_idx]->neighborsCount;

		if (min_cut_curr < min_cut)
		{
			min_cut = min_cut_curr;
		}

		//Free localGraphMap
		for (j = 0; j < NUM_VERTICES; j++)
		{
			free(localGraphMap[j]);
		}
	}
	printf("Final min cut is %d\n",min_cut);
}
