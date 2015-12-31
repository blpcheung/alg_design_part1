//This program creates max heap and implements heap sorting

#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
  This function creates maximum heap
  Input: nums - an array of numbers
         numsSize - size of the array
  Output: max heap 
*/
int *GenMaxHeap(int* nums, int numsSize) {
    
    //create a max heap
    int heapSize = numsSize+1;  //maxHeap[0] is not used

    int *maxHeap = (int *)malloc(sizeof(int)*heapSize);
    int i, temp, child, parent;
    
    maxHeap[1] = nums[0]; 
    for (i = 2; i < heapSize; i++)
    {
        maxHeap[i] = nums[i-1];   //insert at the end
        
        //Compare with its parent
        child = i;
        parent = child/2;
        
        while (maxHeap[parent] < maxHeap[child]) //swap the 2 
        {
            temp = maxHeap[parent];
            maxHeap[parent] = maxHeap[child];
            maxHeap[child] = temp;

            child = parent;
            if (child == 1)   //at the root
                break;
            parent = child/2;
        }    
    }
    
    return maxHeap;
}

/*
  This function implements heap sorting
  Input: max_heap - max heap
         heap_size - size of the heap
  Output: a sorted array (in ascending order)
*/
int *MaxHeapSort(int *max_heap, int heap_size)
{
	int *sorted_array = (int *)malloc(sizeof(int)*(heap_size-1));
	int i, temp, parent, child, child_left, child_right;

	for(i = heap_size-1; i > 2; i--)
	{
	    //Remove the root and put it to the end of the array
		sorted_array[i-1] = max_heap[1];
		//Put the last item in the root
		max_heap[1] = max_heap[i];

		if (i == 3) break;   //only 2 items left

		//Bubble down
		parent = 1;
	    	child_left = parent * 2;
		child_right = parent * 2 + 1;

		if (max_heap[child_left] > max_heap[child_right])   //select the larger child of the two to swap
			child = child_left; 
		else 
			child = child_right;  

		while(max_heap[parent] < max_heap[child])  //swap the root with the max of two children.  Repeat until the heap order is restored.
		{
			temp = max_heap[child];
			max_heap[child] = max_heap[parent];
			max_heap[parent] = temp;

			parent = child;
			child_left = parent * 2;
			child_right = parent * 2 + 1;

			if (child_left > i)     //we are at the bottom level
				break;

			if (child_right > i)    //we still have the left child to compare
			{
				child = child_left;
			}
			else
			{
				if (max_heap[child_left] > max_heap[child_right]) child = child_left; else child = child_right;
			}
		}
	}

	//for the last 2 items
	if (max_heap[1]>max_heap[2])
	{
		sorted_array[1] = max_heap[1];
		sorted_array[0] = max_heap[2];
	}
	else
	{
		sorted_array[1] = max_heap[2];
		sorted_array[0] = max_heap[1];
	}

	return sorted_array;
}

/*This function tests the implementation of max heap and heap sorting*/
void TestMaxHeapSort()
{
	const int numsSize = 19;
	int nums[numsSize];
	int i;
	
	FILE *fr;
	
	fr = fopen("IntegerArray_small.txt","r");

	if (fr == NULL)
	{
		printf("File  not found");
	}
	else
	{
      	  for (i = 0; i<numsSize; i++)
	  {
		fscanf(fr,"%d",&nums[i]);
	  }

	  fclose(fr);

	  int *max_heap = GenMaxHeap(nums, numsSize);

	  for (i = 1; i <=numsSize; i++)  //print the maxheap
	  {
		printf("max_heap[%d]=%d\n",i,max_heap[i]);
	  }

	  int *sorted = (int *)malloc(sizeof(int)*numsSize);
	  sorted = MaxHeapSort(max_heap,numsSize+1);     //sort in ascending order
	
	  for (i = 0; i <numsSize; i++)  //print the maxheap
	  {
		printf("sorted[%d]=%d\n",i,sorted[i]);
	  }
	}
}
