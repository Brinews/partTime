//
//  Test Program 1 for Dynamic Int Queue Class
//
#include <iostream>   // for I/O facilities
#include "queue.h" //  basic_int_queue declarations
using namespace std;

int main (int argc, char * const argv[]) {
        
   bool OK = true ;
   queue q1 ;
   if( ! q1.empty() ) OK = false ;
   q1.enqueue(1);
   if( q1.empty() ) OK = false ;
   q1.enqueue(2);
   q1.enqueue(3);
   q1.enqueue(4);
   if( q1.front() != 1 )  OK = false ;
   q1.dequeue();
   q1.dequeue();
   q1.dequeue();
   if( q1.front() != 4 )  OK = false ;
   if( q1.dequeue() != 4 ) OK = false ;
   if( ! q1.empty() ) OK = false ;

   if( OK ){
      cout << "GOOD" << endl ;
      return 0 ;
   }else{
      cout << "BAD" << endl ;
      return -1 ;
   }
}
