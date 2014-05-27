#include <stdio.h>

struct node
{
	int p;
	struct node *next;
};

void insert(struct node **root)
{
	*root = new node;
	(*root)->p = 100;
	(*root)->next = NULL;
}

int main()
{
	struct node *r = NULL;
	insert(&r);

	printf("%d\n", r->p);

	return 0;
}
