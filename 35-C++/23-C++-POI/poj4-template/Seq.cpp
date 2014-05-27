// Implementation of the simple Sequence class

#include "Seq.h"
#include <string>
#include <iostream>
using namespace std; // for cout, cin 

// Default constructor
Seq::Seq(){
   first = NULL;
   size = 0;
}

// Copy Constructor
Seq::Seq(const Seq& original ){
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

Seq::~Seq(){
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
void Seq::add(int x){
   Node *p = new Node; //temporary node
   // Assign appropriate values to the new node
   p -> data = x;
   p -> next = first;
   // Make first point to the new node
   first = p;	
   size++;
}

// Inserts element x at the given position (or index) in the sequence
void Seq::insertAt(int x, int pos){
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
bool Seq::remove(int x){
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
void Seq::display(){
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
