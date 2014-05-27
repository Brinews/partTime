//
//  classes.cpp
//  lab5
//
//  Created by Ciel Gao on 13-11-7.
//  Copyright (c) 2013年 Ciel Gao. All rights reserved.
//

#include "classes.h"


customer::customer(){
    
}

customer::customer(int a,int b){
    customerID=a;
    arrivaltime=b+randomg();
}

void customer::setarv(int timenow){
    arrivaltime=timenow+randomg();
}

void customer::setde(int t){
    dequeuetime=t;
    waittime=dequeuetime-arrivaltime;
}

int randomg(){
    return 1 + rand()%4;
}

cashier::cashier(customer&first,int timenow){
    current=&first;
    setcus(timenow);
}

void cashier::setcus(int timenow){
    servt=randomg();
    start=timenow;
    end=timenow+servt;
    busy=true;
}


















