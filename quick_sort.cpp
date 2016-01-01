//This program implements quickSort and calculates the total number of comparison

#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 This function puts all the numbers less than the pivot to the left and larger than the pivot to the right of the pivot
 Input: input_array[] - an array of unsorted numbers
        lo - index of the 1st element in the array
        hi - index of the last element in the array
        choose_pivot - index of the array where pivot is chosen
 Output: index of the array where pivot is located
*/
int Partition(int input_array[], int lo, int hi, int choose_pivot)
{
	int p_idx, i, j, temp;
	
	//Choose pivot
	if (choose_pivot == 1)   //Choose the 1st element as pivot
	{
		p_idx = lo;
	}
	else if (choose_pivot == 2)
	{
		p_idx = hi;
	}
	else
	{

		int middle_idx = (hi - lo)/2 + lo;
	
		//find median element as pivot
		int a,b,c;
		a = input_array[lo];
		b = input_array[middle_idx];
		c = input_array[hi];
		
		if ((b >= a && b <= c) || (b >= c && b <= a)) 
		{
			p_idx = middle_idx;
		}
		else if ((c >= a && c <= b) || (c >= b && c <= a)) 
		{
			p_idx = hi;
		}
		else if ((a >= c && a <= b) || (a >= b && a <= c))
		{
			p_idx = lo;
		}
		else
		{
			exit(0);
		}
	}

	//Put the pivot to the first element of the array
	if (p_idx != lo)
	{
	  temp = input_array[lo];
	  input_array[lo] = input_array[p_idx];
	  input_array[p_idx] = temp;
	  p_idx = lo;
	}
	i = lo+1; 

	for (j = lo+1; j <= hi; j++)
	{
		if (input_array[j] < input_array[lo])   //if (input_array[j]<pivot)
		{
		    //swap A[i] and A[j]
			temp = input_array[i];
			input_array[i] = input_array[j];
			input_array[j] = temp;
			i++;
		}
	}

	//swap the pivot with A[i-1]
	temp = input_array[lo];
	input_array[lo] = input_array[i-1];
	input_array[i-1] = temp;

	return i-1;  //array idx where pivot is located
}

/*
 This function performs quick sort and also calculates the number of comparison during quick sort.
 Input: input_array[] - an array of unsorted numbers
        lo - index of the 1st element in the array
        hi - index of the last element in the array
        choose_pivot - index of the array where pivot is chosen
        total_comp - Total number of comparison
*/
void QuickSort(int input_array[], int lo, int hi, int choose_pivot, int *total_comp)
{
	if (hi <= lo) return;

	int pivot_idx = Partition(input_array,lo,hi,choose_pivot);
	QuickSort(input_array,lo,pivot_idx-1,choose_pivot,total_comp);
	if ((pivot_idx -1) > lo)
		*total_comp += (pivot_idx-1) - lo;
	QuickSort(input_array,pivot_idx+1,hi,choose_pivot,total_comp);
	if (hi > pivot_idx+1)
		*total_comp += hi - (pivot_idx+1);
}

void TestQuickSort()
{
	const int array_size = 10000;  //20 for QuickSort_small.txt
	int pivot_chosen = 3, total_comp = 0;
	int array_A[array_size];

	FILE *fr;
	fr = fopen("QuickSort.txt","r");
	if (fr == NULL)
    	{
        	printf("Error Reading File\n");
    	}
	else
	{
		for (int i = 0; i < array_size; i++)
		{
			fscanf(fr,"%d",&array_A[i]);
		}
		fclose(fr);
	
		total_comp = array_size - 1;
		QuickSort(array_A,0,array_size-1,pivot_chosen,&total_comp);

		for (int cnt = 0; cnt < array_size; cnt++)
		{
			printf("After quicksort, array_A[%d] = %d\n",cnt,array_A[cnt]);
		}
		printf("Total comparison = %d",total_comp);
	}
}
