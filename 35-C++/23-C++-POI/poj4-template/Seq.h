// File Seq.h
// Header file for a simple linked-list implementation of a Sequence class
#include <string.h>
#include <iostream>

using namespace std;

template<typename T>
class Seq{
public:
   // Constructors and Destructors
   Seq(); //default construtor
   Seq(const Seq& set); //copy constructor
   ~Seq(); //destructor

   // Adds element data to the front of the sequence.
   void add(T data);

   // Adds element data at indes pos.
   void insertAt(T data, int pos);
   // All existing 
   // Precondition: The sequence is already of length at least pos.
	
   bool remove(T data );
   // removes data from the sequence.
   // makes no change if data is not in the sequence
   // returns true iff data was in the sequence, and has been removed

   void display();
   // prints the sequence ot the screen, as a comma-separated list.

private:
   // A struct is like a class that has no functions/methods. 
   // It has data members, which are accessed using the same notaiton as for classes.
   // List node
   struct Node {
      T data; //list data
      Node *next; //pointer to next item in the list
   };

   Node * first; //Pointer to the first node in the list
   int size; //Records the number of nodes in the list
};

template<typename T>
Seq<T>::Seq()
{
	first = NULL;
	size = 0;
}

// Copy Constructor
template<typename T>
Seq<T>::Seq(const Seq& original ){
   if (original.first == NULL){
      first = NULL;
      size = 0;
   }else{
      first = new Node;
      first->data = original.first->data;
      Node *pNewNode = first ;
      Node *pOldNode = original.first->next;
      // Repeat until the entire list is copied
      while (pOldNode != NULL){
         pNewNode->next = new Node;
         pNewNode = pNewNode->next;
         pNewNode->data = pOldNode->data;;
         pOldNode = pOldNode->next;
      }
      pNewNode->next = NULL;
      size = original.size;
   }
}

template<typename T>
Seq<T>::~Seq(){
   Node *p = first;
   // Traverse the list deleting nodes
   while (p!= NULL){
      first = first->next; // hang on to the rest of the list
      delete p; // De-allocate memory
      p = first; // Go to next node
   }
   first = NULL;
   size = 0;
}

// Adds a node to the start of the sequence, making it the (new) first element
template<typename T>
void Seq<T>::add(T x){
   Node *p = new Node; //temporary node
   // Assign appropriate values to the new node
   p -> data = x;
   p -> next = first;
   // Make first point to the new node
   first = p;	
   size++;
}

// Inserts element x at the given position (or index) in the sequence
template<typename T>
void Seq<T>::insertAt(T x, int pos){
   Node *p;
   Node *newNode;
   // If pos is not a valid index, do nothing.
   if (pos > size){ 
      return ;
   }
   newNode = new Node; //new node
   newNode->data = x;

   // Deal with case when item is to be inserted at the front
   if (pos == 0){
      newNode->next = first;
      first = newNode;
   }else{ // pos > 0
      p = first;
      // Move to position BEFORE insertion point
      for(int i = 0; i < pos-1; i++){
         p = p->next;
      }
      // Insert node
      newNode->next = p->next;
      p->next = newNode;
   }
   size++;
}

// If data occurs in the sequence, removes the first occurrence and returns 
// true, otherwise returns false.
template<typename T>
bool Seq<T>::remove(T x){
   Node *p = first;
   Node *temp;
   // If sequence is empty, just return false.
   if (first == NULL){
      return false;
   }
   // Handle case where target is first
   else if (first->data == x){
      first = first ->next;
      delete p; //currently assigned head
      size--;
      return true;
   }
   // Otherwise traverse the list, looking for data
   else{
      while(p->next != NULL){
         // Check next node for target
         if(p->next->data == x){
            temp = p->next;
            p->next = p->next->next;
            delete temp;
            return true;
         }
         p = p->next;
      }
   }
   return false;
}

// Prints the entire sequence to the screen.
// Most classes should not do their own I/O like this, because this 
// prevents a programmer using the class from controlling the I/O.
// But, it can be convenient for debugging.
template<typename T>
void Seq<T>::display(){
   Node *p = first;
   cout << "["; //Nice format!
   // Traverse the list
   while (p != NULL){
      cout << p -> data; // Print data
      p = p -> next; // Go to next node
      if (p != NULL){
         cout << ","; // Print a comma unless at the end of the list
      }
   }
   cout << "]"; // Don't print a newline - it might not be wanted
}
