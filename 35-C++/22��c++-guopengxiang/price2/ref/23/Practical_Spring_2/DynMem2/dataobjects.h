#ifndef DATAOBJECTS_H_INCLUDED
#define DATAOBJECTS_H_INCLUDED

#include <string>
#include <iostream>

using namespace std;

class BasicData {
    public:
        virtual void Display()=0;
};

class Integer: public BasicData {
    public:
        int Data;
        void Display() { cout << "I am an Integer! My value is: " << Data << endl;}
};

class Double: public BasicData {
    public:
        double Data;
        void Display() { cout << "I am a Double! My value is: " << Data << endl;}
};

class String: public BasicData {
    public:
        string Data;
        void Display() { cout << "I am a String! My value is: " << Data << endl;}
};



#endif // DATAOBJECTS_H_INCLUDED
