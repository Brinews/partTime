#include "dataobjects.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<BasicData*> vect;
    int ch=0;

    while (ch!=4) {
        cout << "What kind of data would you like to enter next?" << endl;
        cout << "(1): An integer" << endl;
        cout << "(2): A double" << endl;
        cout << "(3): A string" << endl;
        cout << "(4): Quit" << endl;
        cout << "Your choice: "; cin >> ch;


        switch (ch) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
        }
    }

}
