/*
###############################
### DO NOT MODIFY THIS FILE ###
###############################
*/

#ifndef SMB_H_
#define SMB_H_

/* type definitions */
typedef struct stage stage;

/* data structure to store stage information */
struct stage
{
    char name[21]; /* stage name */
    stage** pipes; /* set of defined pipes */
    stage* next;   /* pointer to next stage */
    int ncoins;    /* number of coins in the stage */
    int npipes;    /* number of pipes in the stage */
};

#endif
