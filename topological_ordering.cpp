//==================================Topological ordering==============================================

#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct DirectedGraphVertex {
      unsigned int label;  //min. label is 1, not 0
      unsigned int sink_list[NEIGHBORS_MAX_SIZE];
      unsigned int sinkCount;
	  unsigned int src_list[NEIGHBORS_MAX_SIZE];
      unsigned int srcCount;
};

void DFS_1stPass(struct DirectedGraphVertex** graphMap, int node_label, bool explored_chk[], int finish_time[], int* t)
{
	int graphMapIdx = node_label - 1;
	explored_chk[graphMapIdx] = 1;
	int i;

	struct DirectedGraphVertex* node = graphMap[graphMapIdx];

	for(i = 0; i < node->srcCount; i++)
	{
		if (!explored_chk[node->src_list[i]-1])
		{
			int node_j = node->src_list[i];
			DFS_1stPass(graphMap,node_j,explored_chk,finish_time,t);
		}
	}
	//finish_order[*t] = node_label;
	(*t) += 1;
	finish_time[graphMapIdx] = *t; 
	printf("Finish_time[%d] = %d\n",node_label,finish_time[graphMapIdx]);
}

void DFS_2ndPass(struct DirectedGraphVertex** graphMap, int node_label, bool explored_chk[], int *leader_size)
{
	int graphMapIdx = node_label - 1;
	explored_chk[graphMapIdx] = 1;
	int i;

	struct DirectedGraphVertex* node = graphMap[graphMapIdx];

	for(i = 0; i < node->sinkCount; i++)
	{
		if (!explored_chk[node->sink_list[i]-1])
		{
			int node_j = node->sink_list[i];
			*leader_size += 1;
			DFS_2ndPass(graphMap,node_j,explored_chk,leader_size);
		}
	}
}

void TestFindSCC()
{
	FILE *fp;
	
	fp = fopen("SCC_medium.txt","r");

	char *pch;
	const int line_size = 50;
	char line[line_size];
	const int num_vertices = 12;
	struct DirectedGraphVertex *graphMap[num_vertices];
	bool explored_chk[num_vertices];	//indicate if the node has been explored (1) or not (0)
	int finish_time[num_vertices];		//store node_label in the order of finish time 
    int leader[num_vertices];			//store the size of SCC for each leader.  Size 0 means not a leader
	int data,coln_idx,i,j,t=0;

	for (i = 0; i < num_vertices; i++)
	{
		graphMap[i] = NULL;
		explored_chk[i] = false;
	}

    if (fp != NULL){
		int graphMap_idx = 0;
		int node_x, node_y;
		struct DirectedGraphVertex *node;

		while (fgets(line, line_size, fp) != NULL)
		{
			printf("Line %d is %s, ",graphMap_idx,line);
			coln_idx = 0;
			pch = strtok (line," ");   //for each line, read the number with /t as the delimiter
            while (pch != NULL)
            {
				data = atoi(pch);

				if (coln_idx == 0)  //1st column
				{
					node_x = data;
					coln_idx++;
					pch = strtok (NULL," ");
				}
				else         //2nd column
				{
					node_y = data;
					break;
				}	  
			}  //finish reading 1 line

			printf("node_x = %d, node_y = %d\n",node_x,node_y);

			graphMap_idx = node_x-1;
			if (graphMap[graphMap_idx] == NULL)  //The source node is never added to the graphMap
			{
				node = (DirectedGraphVertex *)malloc(sizeof(struct DirectedGraphVertex));
				node->label = node_x;
				node->sinkCount = 0;
				node->srcCount = 0;
				graphMap[graphMap_idx] = node;
			}
			else
			{
				node = graphMap[graphMap_idx];
			}
			node->sink_list[node->sinkCount++] = node_y;

			//Also update the corresponding sink node in graphMap
			graphMap_idx = node_y-1;
			if (graphMap[graphMap_idx] == NULL)  //The sink node is never added to the graphMap
			{
				node = (DirectedGraphVertex *)malloc(sizeof(struct DirectedGraphVertex));
				node->label = node_y;
				node->sinkCount = 0;
				node->srcCount = 0;
				graphMap[graphMap_idx] = node;
			}
			else
			{
				node = graphMap[node_y-1];
			}
			node->src_list[node->srcCount++] = node_x;
		}
        fclose (fp);
    }

	//Verify
	for(i = 0; i < 10; i++)
	{
		struct DirectedGraphVertex *probe = graphMap[i];
		for (j = 0; j<probe->sinkCount; j++)
		{
			printf("For node %d, sink_list[%d] is %d\n",probe->label,j,probe->sink_list[j]);
		}
		for (j = 0; j<probe->srcCount; j++)
		{
			printf("For node %d, src_list[%d] is %d\n",probe->label,j,probe->src_list[j]);
		}

		printf("=========================\n");
	}

	int node_label;    //node_label = [1...num_vertices]
	for (node_label = num_vertices; node_label > 0; node_label--)
	{
		if (!explored_chk[node_label-1])
		{
			DFS_1stPass(graphMap,node_label,explored_chk,finish_time,&t);
		}
	}

	for (i = 0; i < num_vertices; i++)
	{
        //Modify the grpahMap to replace node_label with finish_time and also fwd_list and rev_list
		graphMap[i]->label = finish_time[i];
		for (j = 0; j < graphMap[i]->sinkCount; j++)
		{
			graphMap[i]->sink_list[j] = finish_time[graphMap[i]->sink_list[j]-1];
		}

		for (j = 0; j < graphMap[i]->srcCount; j++)
		{
			graphMap[i]->src_list[j] = finish_time[graphMap[i]->src_list[j]-1];
		}

		//Initialization
		explored_chk[i] = false;
		leader[i] = 0;
	}

	//Copy to a new graphMap so that graphMapIdx = node_label-1
	struct DirectedGraphVertex *graphMapNew[num_vertices];
	for (i = 0; i < num_vertices; i++)
	{
		graphMapNew[graphMap[i]->label-1]=graphMap[i];
	}

	int graphMapIdx;
	for (node_label = num_vertices; node_label > 0; node_label--)
	{
		graphMapIdx = node_label-1;
		if (!explored_chk[graphMapIdx])
		{
			leader[graphMapIdx] = 1;         //set leader which intiates DFS_2ndPass
			DFS_2ndPass(graphMapNew,node_label,explored_chk,&leader[graphMapIdx]);
		}
	}

	//Find the size of each leader
	int count=0;
	for (i = 0; i < num_vertices; i++)
	{
		if (leader[i] > 0 && count <=5)
		{
			node_label = i+1;
			printf("SCC leader = %d, size = %d\n",node_label,leader[i]); 
			count++;
		}
	}
}