#include "Seq.h"
#include <iostream>
using namespace std;

int main() {
   cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
   Seq<int> s;      
   s.add(5);
   s.display();
   cout << endl;

   Seq<string> str;
   str.add("demonstrate");
   str.add("show");
   str.insertAt("view", 1);
   str.display();
   cout << endl;

   Seq<double> dSeq;
   dSeq.add(12.03);
   dSeq.add(1.22);
   dSeq.insertAt(3.4, 1);
   dSeq.display();
   cout << endl;

   dSeq.remove(3.4);
   dSeq.display();
   cout << endl;

   return 0;
}
