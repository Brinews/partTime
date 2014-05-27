#include <stdio.h>
#include <math.h>

#define PI 3.1415926
#define SIZE	101
#define PAGE	25

typedef struct rlc {
	int seqno;
	float R;
	float L;
	float C;
	float f0;
	float Q;
} rlc_t;

/* array of struct to store the input and result */
rlc_t rlc_array[SIZE];

/* global variables to calculate the average */
float f0_average, Q_average;
int rec_num;

/* function define */
void rlc_process(FILE *mu_in);
void rlc_print();


/* function */
void rlc_process(FILE *my_in)
{
	int no;
	float r, l, c;

	f0_average = Q_average = 0.0f;
	rec_num = 0;

	memset(rlc_array, 0, sizeof(rlc_t) * SIZE);

	/* read in record */
	while (fscanf(my_in, "%d,%f,%f,%f", 
				&no, &r, &l, &c) != EOF)
	{
		rlc_array[no].seqno = no;
		rlc_array[no].R = r;
		rlc_array[no].L = l;
		rlc_array[no].C = c;

		rlc_array[no].f0 = 1.0f /( 2*PI*sqrt(l*c));
		rlc_array[no].Q = r * sqrt(c/l);

		rec_num++;

		f0_average += rlc_array[no].f0;
		Q_average += rlc_array[no].Q;
	}

	f0_average = f0_average / rec_num;
	Q_average = Q_average / rec_num;
}

void rlc_print()
{
	int i = 0, cnt = 0;
	int input;

	/* print head */
	printf("Brd NO. RES. Freq.(Hz)        Q\n");
	printf("======= ==============  ============\n");

	/* print body */
	for (i = 1; i < SIZE; i++)
	{
		if (rlc_array[i].seqno != 0) {

			printf("%2d       %.2e          %.2e\n",
					rlc_array[i].seqno, rlc_array[i].f0,
					rlc_array[i].Q);

			cnt++;

			if (cnt % 25 == 0) {
				scanf("%d", &input);
			}
		}
	}

	/* print average */
	printf("Average  %.2e          %.2e\n",
			f0_average, Q_average);
}

int main()
{
	FILE *my_in;
	int count;

	/* open the read data file */
	if (NULL == (my_in = fopen("U:\\datafile.txt", "r")))
	{
		printf("\nError opening input file, program exiting\n");
		printf("\nType a number & press the enter key to finish");
		scanf("%d",&count);
		return(1);
	}

	/* do calculation */
	rlc_process(my_in);

	rlc_print();

	return 0;
}
