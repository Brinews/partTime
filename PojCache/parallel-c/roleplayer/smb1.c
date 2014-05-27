#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smb.h"

#define NAMLEN	21
#define PIPNUM	8
#define BUFLEN	1024

/**
 * Some Constants
 */
typedef enum {
	RET_OK,

	ERR_ILLFORMAT,		/* incorrect input */
	ERR_NAMEXCEED,		/* name length > 20 */
	ERR_NEGATCOIN,		/* negative coin num */
	ERR_PIPEXCEED,		/* pipes number > 8 */
	ERR_DUPDEFINE, 		/* pipes defined multiple  times*/
	ERR_MISDEFINE, 		/* ref to not exist stages */
	ERR_WRONGPIPE,		/* pipe wrong */
	ERR_INVLDMOVE,		/* invalid move */
	ERR_INVLDWAGE,		/* wager more coin than own */

	FAIL_LESSCOIN,		/* lost for no enough coin */
	FAIL_TIMEOUT,		/* time out before reach last stages */
	FAIL_COINOUT,		/* no enough coin in stages */
	FAIL_NOREACH,		/* not complete the maze after all moves */

	SUCC_WONGAME		/* win the game */

} NSMB_STATE;

char *messages[] = {
	"Read: OK",

	"error\n",//	Incorrect input not matching the given input.",
	"error\n",//	Stages name exceed 20 characters.",
	"error\n",//	Stages with negative coins.",
	"error\n",//	Stages with pipes exceed 8.",
	"error\n",//	Pipes defined multiple times.",
	"error\n",//	Reference to stages not defined.",
	"error\n",//	Pipe defined could not fit in the stages.",
	"error\n",//	Invalid move, including no entry.",
	"error\n",//	Invalid wager num, cannot be more than own.",

	"lost\n",//		Reach the final stage without collecting enough coins.",
	"lost\n",//		Not made it to the final stage after the last minutes.",
	"lost\n",//		Player attempts to collect a coin or go through a pipe no exist.",
	"lost\n",//		After all moves, the player not complete the maze.",

	"won\n",
	"",
	"",
};


/**
 * Global Variables 
 * settings include coins, minutes, stages, links
 */
unsigned int nseed_g;
unsigned int ncoin_g;
unsigned int nmins_g;
unsigned int nstages_g;
unsigned int nlinks_g;

char start_stage[NAMLEN], end_stage[NAMLEN];

/* stages list */
stage *head, *tail;
stage *start, *final;

/* read buffer */
char buf[BUFLEN];

/**
 * Function Prototype
 */
NSMB_STATE nsmb_doinit();		/* read game conditions */
NSMB_STATE nsmb_doruning();		/* simulating the game */

/**
 * help functions
 */
stage* getstage(char *name, stage *head);
void freestagelist(stage *head, stage *tail);
int pipe2stage(stage* s, stage* t);

/**
 * find stage by name
 * @param (name, head)
 * @return NULL if no found
 */
stage* getstage(char *name, stage *head)
{
	stage *cur = head;

	if (NULL != cur)
	{
		if (strcmp(cur->name, name) == 0)
			return cur;

		cur = cur->next;
		while (cur != head)
		{
			if (strcmp(cur->name, name) == 0)
				return cur;

			cur = cur->next;
		}
	}

	return NULL;
}

/**
 * free the stage list
 * @param (head, tail)
 */
void freestagelist(stage *head, stage *tail)
{
	while (head != tail)
	{
		stage* next = head->next;
		free(head);
		free(head->pipes);
		head = next;
	}

	if (NULL != head) {
		free(head);
		free(head->pipes);
	}
}

/**
 * pipe two stage
 * the pipe link should not exceed the stage's npipes
 * @param (s, t)
 * @return 0 for ok
 */
int pipe2stage(stage* s, stage* t)
{
	int i;
	for (i = 0; i < s->npipes; i++)
		if (NULL == s->pipes[i])
			break;

	if (i == s->npipes) return -1;
	s->pipes[i] = t;

	for (i = 0; i < t->npipes; i++)
		if (NULL == t->pipes[i])
			break;

	if (i == t->npipes) return -1;
	t->pipes[i] = s;

	return 0;
}

/**
 * Read the given information
 * @return NSMB_STATE
 */
NSMB_STATE nsmb_doinit()
{
	int i, j;
	int ret;
	stage *sa, *next;
	char name[NAMLEN], dest[NAMLEN];
	char delimiter[NAMLEN];
	int ncoins, npipes;

	/* 1. read first section */
	fgets(buf, BUFLEN, stdin);
	if (NULL == buf || strlen(buf) == 0)
		return ERR_ILLFORMAT;

	ret = sscanf(buf, "%u%u%u%u%u", 
			&nseed_g, &ncoin_g, &nmins_g, &nstages_g, &nlinks_g);
	if (ret < 5)
		return ERR_ILLFORMAT;

	/* 2. read stages */
	for (i = 0; i < nstages_g; i++)
	{
		fgets(buf, BUFLEN, stdin);
		if (NULL == buf || strlen(buf) == 0)
			return ERR_ILLFORMAT;

		ret = sscanf(buf, "%s%d%d", 
						  name, &npipes, &ncoins);
		if (ret < 3)
			return ERR_ILLFORMAT;

		if (strlen(name) >= NAMLEN)
			return ERR_NAMEXCEED;

		if (ncoins < 0)
			return ERR_NEGATCOIN;

		/*
		if (npipes > PIPNUM)
			return ERR_PIPEXCEED;
		*/

		if (getstage(name, head) != NULL)
			return ERR_DUPDEFINE;

		/* add to link list */
		sa = malloc(sizeof(struct stage));
		memset(sa, 0, sizeof(struct stage));
		strcpy(sa->name, name);
		sa->ncoins = ncoins;
		sa->npipes = npipes;
		sa->pipes = malloc(sizeof(stage *) * npipes);

		for (j = 0; j < npipes; j++)
			sa->pipes[j] = NULL;

		if (NULL == head)
		{
			head = tail = sa;
		}
		else 
		{
			tail->next = sa;
			tail = sa;
		}

		tail->next = head;
	}

	/* 3. read the pipes */
	for (i = 0; i < nlinks_g; i++)
	{
		fgets(buf, BUFLEN, stdin);
		if (NULL == buf || strlen(buf) == 0)
			return ERR_ILLFORMAT;

		ret = sscanf(buf, "%s%s%s",name, delimiter, dest);
		if (ret < 3) 
			return ERR_ILLFORMAT;

		if (strlen(delimiter) != 1 && delimiter[0] != '~')
			return ERR_ILLFORMAT;

		/* find the piped stage by name */
		sa = getstage(name, head);
		next = getstage(dest, head);
		if (NULL == sa || NULL == next)
			return ERR_MISDEFINE;

		/* pipe link */
		ret = pipe2stage(sa, next);
		if (-1 == ret)
			return ERR_WRONGPIPE;
	}

	/* 4. read first and last stage name */
	fgets(buf, BUFLEN, stdin);
	if (NULL == buf || strlen(buf) == 0)
		return ERR_ILLFORMAT;

	ret = sscanf(buf, "%s%s", name, dest);
	start = getstage(name, head);
	final = getstage(dest, head);
	if (NULL == start || NULL == final)
		return ERR_MISDEFINE;

	return RET_OK;
}

/**
 * running the game
 */
NSMB_STATE nsmb_doruning()
{
	int ret;
	int rmin = nmins_g, ccoin = 0;
	stage *cur = start;
	int pmin;

	char command[BUFLEN], wagestate[BUFLEN];
	int pno, colnum, wagnum;

	srand(nseed_g);

	fprintf(stdout, "%d %d %s %d %d\n",
			rmin, ccoin, cur->name, cur->npipes, cur->ncoins);

	/* read in the steps */
	while (NULL != fgets(buf, BUFLEN, stdin)
			&& NULL != cur)
	{
		/* just a flag */
		pmin = rmin;

		/* interpret the command */
		/* 1. move next */
		if (buf[0] == 'n') 
		{
			sscanf(buf, "%s", command);
			if (strcmp(command, "next") == 0) 
			{
				cur = cur->next;
				rmin--;
			}
		}

		/* 2. move through pipe n */
		if (buf[0] == 'p')
		{
			ret = sscanf(buf, "%s%d", command, &pno);
			if (ret < 2 
					|| strcmp(command, "pipe") != 0
					|| pno < 0)
				return ERR_ILLFORMAT;

			if (pno > cur->npipes)
				cur = NULL;
			else {
				cur = cur->pipes[pno-1];
			}
			rmin--;
		}

		/* 3. collect coin */
		if (buf[0] == 'c')
		{
			ret = sscanf(buf, "%s%d", command, &colnum);
			if (ret < 2
					|| strcmp(command, "collect") != 0
					|| colnum < 0)
				return ERR_ILLFORMAT;

			rmin -= colnum;
			ccoin += colnum;
			cur->ncoins -= colnum;
		}

		/* 4. wager */
		if (buf[0] == 'w')
		{
			ret = sscanf(buf, "%s%d%s", command, &wagnum, wagestate);
			if (ret < 3 
					|| strcmp(command, "wager") != 0
					|| (strcmp(wagestate, "even") != 0 &&
							strcmp(wagestate, "odd") != 0)
					|| wagnum < 0)
				return ERR_ILLFORMAT;

			if (wagnum > ccoin)
				return ERR_INVLDWAGE;

			ret = rand();
			//ret = 2;
			if ((ret % 2 == 0 && strcmp(wagestate, "even") == 0)
			 	|| (ret % 2 == 1 && strcmp(wagestate, "odd") == 0))
			{
				/* wager win */
				ccoin += wagnum;
			}
			else
			{
				/* wager lost */
				ccoin -= wagnum;
			}

			rmin--;
		}

		/* others: illegal */
		if (pmin == rmin)
			return ERR_ILLFORMAT;

		/* after each decision */
		if (NULL == cur)
			return ERR_INVLDMOVE;

		if (cur->ncoins < 0)
			return FAIL_COINOUT;

		if (rmin < 0)
			return FAIL_TIMEOUT;

		fprintf(stdout, "%d %d %s %d %d\n",
				rmin, ccoin, cur->name, cur->npipes, cur->ncoins);
	}

	/* judge the last status */
	if (NULL == cur || final != cur)
		return FAIL_NOREACH;

	if (ccoin < ncoin_g)
		return FAIL_LESSCOIN;

	return SUCC_WONGAME;
}

/**
 * Main function
 */
int main(void)
{
	NSMB_STATE state = nsmb_doinit();

	if (state != RET_OK)
	{
		fprintf(stdout, "%s", messages[state]);
		return -1;
	}

	state = nsmb_doruning();
	fprintf(stdout, "%s", messages[state]);

	/* free the malloced */
	freestagelist(head, tail);

	return 0;
}
