#include "queue.h"
#include <stdio.h>

#include <assert.h>

queue::queue()
{
	front_p = back_p = NULL;
	current_size = 0;
}

void queue::enqueue(int item)
{
	node *p = new node(item, NULL);

	if (NULL == p) {
		assert(p);
	}

	if (NULL == back_p)
	{
		back_p = p;
	}
	else 
	{
		back_p->next = p;
		back_p = back_p->next;
	}

	if (NULL == front_p)
		front_p = p;

	current_size++;
}

int queue::dequeue()
{
	int r;
	assert(!empty());

	node *p = front_p;
	front_p = front_p->next;
	r = p->data;

	//delete no use node from the queue
	delete p;
	p = NULL;
	current_size--;

	//dequeue the last node
	if (0 == current_size) 
		back_p = NULL;

	return r;
}

int queue::front()
{
	assert(!empty());

	return front_p->data;
}

bool queue::empty()
{
	return current_size == 0;
}

int queue::size()
{
	return current_size;
}

int queue::remove(int item)
{
	int c = 0;

	node *p = front_p;
	if (NULL == p) return 0;

	node *q = p->next;

	while (NULL != q) 
	{
		if (item == q->data)
		{
			//when remove last node
			if (q == back_p)
				back_p = p;

			//remove the node
			p->next = q->next;
			q->next = NULL;
			delete q;
			q = p->next;

			//to count
			c++;
			current_size--;
		}
		else 
		{
			p = q;
			q = q->next;
		}
	}

	//when front_p->data == item
	if (front_p->data == item)
	{
		c++;
		current_size--;

		p = front_p;
		front_p = front_p->next;
		delete p;

		if (current_size == 0)
		{
			front_p = back_p = NULL;
		}
	}

	return c;
}
