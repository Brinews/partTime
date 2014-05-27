/***********************************************************
   heap.cpp -- Implementation File for Basic Heap-of-Ints
************************************************************/
#include "heap.h"

// Constructors and Destructor

Heap::Heap(void){ // New empty Heap with default capacity.
   h_capacity = 10;
   A = new int[h_capacity];
   h_size = 0;
}

Heap::Heap(int c){ // New empty Heap with capacity c.
   // Complete this.
	assert(c <= 0);

	h_capacity = c;
	A = new int[h_capacity];
	assert(NULL == A);

	h_size = 0;
}

Heap::Heap(int * B, int s, int c){ // New Heap with capacity c containing first s elements of B.
   // Complete this.
	assert(c <= s || s < 0 || NULL == B);

	h_capacity = c;
	A = new int[h_capacity];
	assert(NULL == A);

	h_size = 0;
	for (int i = 0; i < s; i++)
		A[h_size++] = B[i];

	make_heap();
}

Heap::Heap( const Heap & H ){ // Copy constructor.
   // Complete this.
	h_capacity = H.h_capacity;
	h_size = 0;

	A = new int[h_capacity];
	assert (NULL == A);

	for (int i = 0; i < H.h_size; i++)
	{
		A[i] = H.A[i];
	}

	/**
	while (!H.empty())
	{
		insert(H.extract_min());
	}
	**/
}

// Destructor
Heap::~Heap(){
   delete[] A;
}


// Operators

Heap Heap::operator+( const Heap & Right_H) const { 
   // New Heap with combined contents and size of operands.
   int new_capacity = h_capacity + Right_H.h_capacity ;
   Heap New_H( new_capacity );

   // Complete this.
   for (int i = 0; i < h_size; i++)
	   New_H.insert(A[i]);

   for (int i = 0; i < Right_H.h_size; i++)
	   New_H.insert(Right_H.A[i]);

   /**
   while (!Right_H.empty())
   {
	   insert(Right_H.extract_min());
   }
   **/

   return New_H ;
}

// Modifiers

void Heap::insert(int x){
// Inserts x into the heap.
   A[h_size] = x;
   trickle_up(h_size);
   h_size ++;
}

// Repairs the heap ordering invariant after adding a new element.
// Initial call should be trickle_up(h_size-1).
void Heap::trickle_up(int v){
   // Complete this.
	int p = (v - 1)/2;
	if (p < 0 || v == 0) return;

	if (A[p] > A[v])
	{
		int tmp = A[p];
		A[p] = A[v];
		A[v] = tmp;

		trickle_up(p);
	}
}

// Removes and returns minimum element.
int Heap::extract_min(){
   int temp = A[0];
   A[0] = A[--h_size];
   trickle_down(0);   
   return temp;
}

// Repairs the heap ordering invariant after replacing the root.
// Normal initial call should be trickle_down(0).
// trickle_down(i) performs the repair on the subtree rooted a A[i].
// make_heap() call trickle_down(i) for i (h_size/2)-1 down to 0.
void Heap::trickle_down(int i){
   // Complete this.
	int n = 2*i + 1;

	if (n >= h_size) return;

	//min child
	if (n+1 < h_size && A[n+1] < A[n])
		n = n+1;

	//swap
	if (A[i] > A[n])
	{
		int tmp = A[i];
		A[i] = A[n];
		A[n] = tmp;

		trickle_down(n);
	}
}

// Turns A[0] .. A[size-1] into a heap.
void Heap::make_heap(){
   for( int i = (h_size/2)-1; i>=0 ; i-- ) trickle_down(i);  
}

// Prints contents of heap to standard output (for testing).
void Heap::display(){
     for(int i = 0; i < h_size ; ++i) cout << A[i] << ", " ;
}
