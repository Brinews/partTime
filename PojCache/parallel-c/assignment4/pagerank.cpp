#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <pthread.h>

#include "pagerank.h"

/**
 * thresh^2
 */
double e_threshold(double *pt, double *qt, int npages)
{
	int i;
	double thresh = 0.0;

	for (i = 0; i < npages; i++)
	{
		thresh += (qt[i] - pt[i])*(qt[i] - pt[i]);
		/* update */
		pt[i] = qt[i];
	}

	return thresh;
}

void print_pagerank(double *pt, int npages, list *plist)
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
	double *pt, *qt; /* pagerank vector */
	node *head, *inode;
	list *link;
	double thresh;
	double nt, ut;

	/* alloc in/out */
	int *out = (int *) malloc(npages * sizeof(int));

	/* alloc pagerank vector */
	qt = (double *)malloc(npages * sizeof(double));
	pt = (double *)malloc(npages * sizeof(double));
	for (i = 0; i < npages; i++)
	{
		qt[i] = 0;
		pt[i] = 1.0F / npages;
	}

	/* sum adjcents */
	head = plist->head;
	for (i = 0; i < npages; i++)
	{
		out[i] = head->page->noutlinks;
		head = head->next;
	}

	/******== begin iteration **/

	nt = (1 - dampener)/npages;
	do
	{
		head = plist->head;

		for (i = 0; i < npages; i++)
		{
			ut = 0.0F;

			link = head->page->inlinks;

			if (NULL != link)
			{
				inode = link->head;
				while (NULL != inode)
				{
					j = inode->page->index;

					ut += pt[j]/out[j];

					inode = inode->next;
				}
			}

			qt[i] = nt + dampener * ut;

			head = head->next;
		}

	} while ((thresh = e_threshold(pt, qt, npages)) > EPSILON*EPSILON);

	/******== end iteration **/

	print_pagerank(qt, npages, plist);
	
	/* free in/out */
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
