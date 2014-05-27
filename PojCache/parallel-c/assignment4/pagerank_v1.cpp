#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <pthread.h>

#include "pagerank.h"

/**
 * thresh^2
 */
float e_threshold(float *pt, float *qt, int npages)
{
	int i;
	float thresh = 0.0;

	for (i = 0; i < npages; i++)
	{
		thresh += (qt[i] - pt[i])*(qt[i] - pt[i]);

		/* update */
		pt[i] = qt[i];
	}

	return thresh;
}

void print_pagerank(float *pt, int npages, list *plist)
{
	node *head;
	int i;

	/* print out pt */
	head = plist->head;
	for (i = 0; i < npages; i++)
	{
		printf("%s %.4lf\n", head->page->name, pt[i]);
		head = head->next;
	}
}

void pagerank(list* plist, int ncores, int npages, int nedges, double dampener) 
{

    /*
        to do:
        - implement this function
        - implement any other necessary functions
        - implement any other useful data structures
    */
	
	int i, j;
	float *pt, *qt; /* pagerank vector */
	node *head, *inode;
	list *link;
	float thresh;
	float nt, ut;

	/* alloc in/out */
	int **inM = (int **) malloc(npages * sizeof(int *));
	int *out = (int *) malloc(npages * sizeof(int));

	for (i = 0; i < npages; i++)
	{
		inM[i] = (int *) malloc(npages * sizeof(int));
		memset(inM[i], 0, sizeof(int) * npages);
	}

	/* alloc pagerank vector */
	qt = (float *)malloc(npages * sizeof(float));
	pt = (float *)malloc(npages * sizeof(float));
	for (i = 0; i < npages; i++)
	{
		qt[i] = 0;
		pt[i] = 1.0F / npages;
	}

	/* sum adjcents */
	head = plist->head;
	for (i = 0; i < npages; i++)
	{
		link = head->page->inlinks;
		out[i] = head->page->noutlinks;

		if (NULL != link)
		{
			inode = link->head;
			while (NULL != inode)
			{
				j = inode->page->index;
				inM[i][j] = 1;

				inode = inode->next;
			}
		}


		head = head->next;
	}

	/******== begin iteration **/

	nt = (1 - dampener)/npages;
	do
	{
		for (i = 0; i < npages; i++)
		{
			ut = 0.0F;

			for (j = 0; j < npages; j++)
			{
				if (inM[i][j] == 1)
				{
					ut += pt[j]/out[j];
				}
			}

			qt[i] = nt + dampener * ut;
		}

		//print_pagerank(qt, npages, plist);
		//printf("\n\n");
	} while ((thresh = e_threshold(pt, qt, npages)) > EPSILON*EPSILON);

	/******== end iteration **/

	print_pagerank(qt, npages, plist);

	//system("pause");
	
	/* free in/out */
	for (i = 0; i < npages; i++)
		free(inM[i]);

	free(inM);
	free(out);

	/* free pt */
	free(pt);
	free(qt);
}

/*
######################################
### DO NOT MODIFY BELOW THIS POINT ###
######################################
*/

int main(void) {

    /*
    ######################################################
    ### DO NOT MODIFY THE MAIN FUNCTION OR HEADER FILE ###
    ######################################################
    */

    list* plist = NULL;

    double dampener;
    int ncores, npages, nedges;

    /* read the input then populate settings and the list of pages */
    read_input(&plist, &ncores, &npages, &nedges, &dampener);

    /* run pagerank and output the results */
    pagerank(plist, ncores, npages, nedges, dampener);

    /* clean up the memory used by the list of pages */
    page_list_destroy(plist);

    return 0;
}
