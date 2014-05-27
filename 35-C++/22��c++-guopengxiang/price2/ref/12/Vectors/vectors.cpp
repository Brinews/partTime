#include <vector>
#include <iostream>

using namespace std;

void display(vector<int> vec) {
    int i;
    for (i=0;i<vec.size()-1;i++) {              // the size is the NUMBER of elements in the vector - the first element is at the oth position though!
        cout << vec.at(i) <<",";
    }
    cout << vec.at(vec.size()-1) << endl;
}

class Data {
    public:
        int dat;
        void display() {cout << dat << ",";}
};

int main() {
    //vector<int> vec(10);
    //int i;

    //for (i=0;i<10;i++) vec.at(i)=2*i+1;

    //display(vec);
    //vec.push_back(100);
    //vec.insert(vec.begin()+5,1000);
    //display(vec);
    //vec.erase(vec.end()-3);             // vec.end() refers to the element of the vector AFTER the last element!!!
    //display(vec);
    //vec.erase(vec.begin()+3,vec.end()-2);   // if you erase an interval, it will erase the first pointed element BUT NOT THE LAST!
    //display(vec);
    //vec.pop_back();
    //display(vec);

    //vector<Data*> vec2;

    //for (i=0;i<10;i++) vec2.push_back(new Data);

    //for (i=0;i<10;i++) vec2.at(i)->dat=3*i-2;

    //for (i=0;i<10;i++) vec2.at(i)->display();

    //for (i=0;i<10;i++) delete vec2.at(i);

    // One way to store objects in a vactor: we store the objects themselves
    // In this case C++ is responsible to dispose of unused objects

/*    vector<Data> vect;
    Data* pData;
    int i;

    for (i=0;i<10;i++) {
        pData=new Data;
        pData->dat=i;
        vect.push_back(*pData);
    }

    for (i=0;i<10;i++) {
        vect.at(i).display();
    }

    vect.pop_back();            // here we get rid of the last OBJECT, so that's fine

    vector<Data>::iterator it;

    for (it=vect.begin();it<vect.end();it++) {
        it->display();
    }
    */

    // The other way is to store the POINTERS to the objects
    // In this case we are responsible to dispose of unused objects

/*    vector<Data*> vect;
    Data* pData;
    int i;

    for (i=0;i<10;i++) {
        pData=new Data;
        pData->dat=i;
        vect.push_back(pData);
    }

    for (i=0;i<10;i++) {
        vect.at(i)->display();
    }

//    vect.pop_back();            // here we get rid of the last POINTER, this is a big MISTAKE - the object will remain in the memory forever!

    pData=vect.at(vect.size()-1);   // this is the correct way to do it!
    vect.pop_back();
    delete pData;

    vector<Data*>::iterator it;

    for (i=0;i<vect.size();i++) {
        pData=vect.at(i);
        pData->display();
        delete pData;
    }*/

}
