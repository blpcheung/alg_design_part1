/*===================================================
Maintain the median value as new items are inserted to the array

Input argument:
	heap: minHeap or maxHeap with array size = max input size + 1
	new_item: new item going to be added to the heap
	max: max = 0, heap is minHeap
	     max = 1, heap is maxHeap
====================================================*/

#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int insertAndGetRootFromHeap(int* heap_array, int new_item, bool max)
{
    //Find the last location where the new_item is inserted
	int new_item_loc = ++heap_array[0];     //heap[0] stores the size of heap.  Size of heap_array is 1 more than size of heap  

	heap_array[new_item_loc] = new_item;   //insert at the end

	//Bubble up
	int temp, child, parent;
	bool swap;

	child = new_item_loc;
    parent = child/2;

	if (child == 1)     //First element is inserted
		return heap_array[1];

	//Check to see if swapping between parent and child is required
	if (max == false)   //minHeap
	{
		swap = (heap_array[parent] > heap_array[child])? true: false;
	}
	else  //maxHeap
	{
        swap = (heap_array[parent] < heap_array[child])? true: false;
	}

	//Compare with its parent
    while (swap) //swap the 2 
    {
        temp = heap_array[parent];
        heap_array[parent] = heap_array[child];
        heap_array[child] = temp;

        child = parent;
        if (child == 1)   //at the root
            break;

        parent = child/2;

		if (max == false)   //minHeap
		{
		  swap = (heap_array[parent] > heap_array[child])? true: false;
		}
		else  //maxHeap
		{
          swap = (heap_array[parent] < heap_array[child])? true: false;
		}
    }    

	return heap_array[1];    //return min/max value
}

int extractRootFromHeap(int* heap_array, bool max)
{
	if (heap_array[0] == 0)   //heap_array is empty
		return -1;            //return invalid value

	int i, root, parent, child, child_left, child_right, temp;
	bool swap;

	//Remove the root
	root = heap_array[1];

	//Put the last item in the root
	heap_array[1] = heap_array[heap_array[0]];
	heap_array[0]--;   //update the heap size

	//Bubble down
	parent = 1;
	child_left = parent * 2;
	child_right = parent * 2 + 1;

	if (child_left > heap_array[0])     //we are at the bottom level
		return root;

	if (child_right > heap_array[0])    //we still have the left child to compare
	{
		child = child_left;
	}
	else
	{
		if (max == true)   //This is maxHeap
		{
			child = (heap_array[child_left] > heap_array[child_right]) ? child_left : child_right;   //select the larger child of the two to swap
		}
		else    //This is minHeap
		{
			child = (heap_array[child_left] < heap_array[child_right]) ? child_left : child_right;   //select the smaller child of the two to swap
		}
	}

	if (max == true)   //This is maxHeap
	{
		swap = (heap_array[parent] < heap_array[child])? true: false;
	}
	else    //This is minHeap
	{
		swap = (heap_array[parent] > heap_array[child])? true: false;
	}

	while(swap)  //swap the parent with the min/max of two children.  Repeat until the heap order is restored.
	{
		temp = heap_array[child];
		heap_array[child] = heap_array[parent];
		heap_array[parent] = temp;

		parent = child;
		child_left = parent * 2;
		child_right = parent * 2 + 1;

		if (child_left > heap_array[0])     //we are at the bottom level
			break;

		if (child_right > heap_array[0])    //we still have the left child to compare
		{
			child = child_left;
		}
		else
		{
			if (max == true)   //This is maxHeap
			{
				child = (heap_array[child_left] > heap_array[child_right]) ? child_left : child_right;   //select the larger child of the two to swap
			}
			else    //This is minHeap
			{
				child = (heap_array[child_left] < heap_array[child_right]) ? child_left : child_right;   //select the smaller child of the two to swap
			}
		}

		if (max == true)   //This is maxHeap
		{
			swap = (heap_array[parent] < heap_array[child])? true: false;
		}
		else    //This is minHeap
		{
			swap = (heap_array[parent] > heap_array[child])? true: false;
		}
	}
	
	return root;
}

void TestMedianMaintain()
{
	const int inputSize = 10000;
	int input[inputSize];
	int i;
	
	FILE *fr;
	
	fr = fopen("Median.txt","r");

	if (fr == NULL)
	{
		printf("File  not found");
	}
	else
	{
      for (i = 0; i<inputSize; i++)
	  {
		fscanf(fr,"%d",&input[i]);
	  }

	  fclose(fr);
	}

	int k,m_k_idx, new_item, minValue, maxValue, median_sum, median;
	int minHeap[inputSize+1];     //contain the 2nd half of the input  
	int maxHeap[inputSize+1];     //contain the 1st half of the input
	//Initialize heap size to 0
	minHeap[0] = 0;  
	maxHeap[0] = 0;

	//For the 1st input, just put to minHeap (2nd half)
	new_item = input[0];
	minValue = insertAndGetRootFromHeap(minHeap, new_item, 0);
	//printf("Put the new item to the 2nd half, min value = %d, minHeap size = %d\n",minValue,minHeap[0]);
	median = new_item;
	median_sum = median;

	for (i = 1; i < inputSize; i++)
	{ 
      //printf("Median = %d, Median sum = %d\n",median,median_sum);
	  new_item = input[i]; 
	  
	  if (new_item < minHeap[1])   //input is less than the min value of the 2nd half
	  {
		maxValue = insertAndGetRootFromHeap(maxHeap, new_item, 1);     //put the new item to the 1st half
		//printf("Put the new item to the 1st half, max value = %d, maxHeap size = %d\n",maxValue,maxHeap[0]);
	  }
	  else
	  {
		minValue = insertAndGetRootFromHeap(minHeap, new_item, 0);
		//printf("Put the new item to the 2nd half, min value = %d, minHeap size = %d\n",minValue,minHeap[0]);
	  }

	  //Rebalance if needed
	  if (abs(minHeap[0]-maxHeap[0])>=2)
	  {
		  if (maxHeap[0] > minHeap[0])       //The 1st half is greater than the 2nd half
		  {
			  new_item = extractRootFromHeap(maxHeap, 1);         //remove the root from the 1st half
			  minValue = insertAndGetRootFromHeap(minHeap, new_item, 0);    //insert to the 2nd half
		  }
		  else
		  {
			  new_item = extractRootFromHeap(minHeap, 0);
			  maxValue = insertAndGetRootFromHeap(maxHeap, new_item, 1);
		  }
	  }

	  //Get the new median and calculate the sum
	  k = minHeap[0]+maxHeap[0];
	  if (k%2 == 1)    //k is odd 
	  {
		  m_k_idx = (k+1)/2;
	  }
	  else  //k is even
	  {
          m_k_idx = k/2;
	  }

	  if (m_k_idx == maxHeap[0])  //compare against how many elements in the 1st half
	  {
		  median = maxHeap[1];
	  }
	  else
	  {
		  median = minHeap[1];
	  }
	  median_sum += median;
	}

	int answer = (median_sum % 10000);

	printf("Medium sum = %d, answer = %d\n",median_sum,answer);
}