#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
  This function sorts the aux_array and put the result into the sorted array using merge sort. 
  It also counts the number of times inversion happens between the 1st and 2nd half of the array
  Input: sorted_array[] - an array of unsorted items before calling this function and sorted items after this function is executed
         aux_array[] - an empty array which is used as a temporary storage during merge sort.  It has the same size as the sorted array
         lo - array index of the 1st element
         mid - array index of the middle element
         hi - array index of the last element
  Output: The number of inversions between the first and second half of the sorted array
*/
uint32 merge_count_split(int sorted_array[], int aux_array[], int lo, int mid, int hi)
{
	int i,j,k;
	uint32 count_inv=0;
	i = lo;
	j = mid+1;

	//copy the array from sorted to aux array first
	for (k = lo; k<= hi; k++)
	{
		aux_array[k] = sorted_array[k];
	}
	for (k = lo; k <= hi; k++)  //from array_idx 0 to 5
	{	
		if (i > mid)     //just copy the rest of 2nd half to output
		{
			sorted_array[k] = aux_array[j++];
		}
		else if (j > hi)  //just copy the rest of 1st half to output
		{
			sorted_array[k] = aux_array[i++];
		}
		else if (aux_array[i]<=aux_array[j])  //copy the element from the 1st half
		{
			sorted_array[k]=aux_array[i++];
		}
		else  //copy the element from 2nd half, count_inversion here
		{
			sorted_array[k]=aux_array[j++];
			count_inv += mid - i + 1;//num of remaining elements in 1st half
			if (count_inv < 0)
			{
				printf("count_inv is negative, mid=%d, i=%d\n",mid,i);
				getchar();
				exit(0);
			}
		}
	}
	return count_inv;
}

/*
  This is the main function for the merge sort algorithm.
  Input: sorted_array[] - an array of unsorted items before calling this function and sorted items after this function is executed
         aux_array[] - an empty array which is used as a temporary storage during merge sort.  It has the same size as the sorted array
         lo - array index of the 1st element
         hi - array index of the last element
  Output: The number of inversions between the first and second half of the sorted array
*/
uint32 sort_count_inversion(int sorted_array[], int aux_array[], int lo, int hi)
{
	if(hi <= lo)
	{
	  return 0;   //base case
	}

	int x,y,z;
	int mid = lo + (hi-lo)/2;
	x = sort_count_inversion(sorted_array,aux_array,lo,mid);
	y = sort_count_inversion(sorted_array,aux_array,mid+1,hi);
	z = merge_count_split(sorted_array,aux_array,lo,mid,hi);
	return (x+y+z);
}

/*This function tests the merge sort algorithm*/
void count_inversion_test()
{
	FILE *fr;
	const int line_size = 100000;
	int line[line_size],i; 

	fr = fopen("IntegerArray.txt","r");
	if (fr == NULL)
        {
          printf("Error Reading File\n");
        }
	else
	{
	  for (i = 0; i < line_size; i++)  //for char, while(fgetc(line, line_size, fr) != EOF), for string, while(fgets(line, line_size, fr) != NULL)
	  {
		fscanf(fr,"%d",&line[i]);  
	  }
	  fclose(fr);

	  uint32 total_inv;
	  int aux_array[line_size];

	  total_inv = sort_count_inversion(line,aux_array,0,line_size-1);
	  printf("The number of inversion is %lu\n",total_inv);
	  
	  for (int cnt = 0; cnt < array_size; cnt++)
	  {
		printf("After sorting, output_array[%d] = %d\n",cnt,line[cnt]);
	  }
	}
}
