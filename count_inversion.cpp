#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef  unsigned char      boolean;     /* Boolean value type. */
typedef  signed char        int8;        /* Signed 8  bit value */
typedef  unsigned char      uint8;       /* Unsigned 8  bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  signed long        int32;       /* Signed 32 bit value */
typedef  unsigned long      uint32;      /* Unsigned 32 bit value */
typedef  signed long long   int64;       /* Signed 64 bit value */
typedef  unsigned long long uint64;      /* Unsigned 64 bit value */

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
	  //array_A[0] = 1; array_A[1] = 3; array_A[2]=5; array_A[3]=2; array_A[4]=4; array_A[5] = 7; array_A[6]=6;

	  total_inv = sort_count_inversion(line,aux_array,0,line_size-1);
	  printf("The number of inversion is %lu\n",total_inv);
	  /*for (int cnt = 0; cnt < array_size; cnt++)
	  {
		printf("output_array[%d] = %d\n",cnt,line[cnt]);
	  }*/
	}
}