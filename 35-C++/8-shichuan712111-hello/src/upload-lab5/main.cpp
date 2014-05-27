//
//  main.cpp
//  lab5
//
//  Created by Ciel Gao on 13-11-7.
//  Copyright (c) 2013年 Ciel Gao. All rights reserved.
//

#include <iostream>
#include "classes.h"
#include "stdlib.h"


int main()
{
    
    int cnum=1;
    int served=0;
    int t=0;
    int twait=0;
    int maxwt=0;
    int tserv=0;
    cout<<"Simulation started"<<endl;
    customer serving(cnum,t);
    t=serving.arrivaltime;
    cout<<"Customer 1 arrived at "<<serving.arrivaltime<<" mins"<<endl;
    cashier one(serving,t);
    cout<<"Customer 1 began service at "<<t<<" mins"<<endl;
    customer next(++cnum,t);
    queue<customer> line1(t);
    
    while(t<=720){
      if(t==next.arrivaltime)
      {line1.enqueue(next,t);
          cnum++;
          next.customerID++;
          next.setarv(t);
          
      }
        if(one.busy==false)
        {if(line1.dequeue(serving)==true)
        {   one.setcus(t);
            cout<<"Customer "<<serving.customerID<<" began service at "<<t<<" mins"<<endl;
        }
        }
        if(t==one.end){
            served++;
            tserv=tserv+one.servt;
            cout<<"Customer "<<serving.customerID<<" ended service at "<<t<<" mins"<<endl;
           if(line1.dequeue(serving)==true)
           {serving.setde(t);
               twait=twait+serving.waittime;
               if(maxwt<serving.waittime)
                   maxwt=serving.waittime;
               one.setcus(t);
            cout<<"Customer "<<serving.customerID<<" began service at "<<t<<" mins"<<endl;
           }
           else one.busy=false;
            
        }
        
        t++;
        
        
        
    }
    
    
    cout<<"Simulation ended"<<endl;
    cout<<"Served "<<served<<" customers"<<endl;
    cout<<"Left in line "<<line1.qnum<<" customers"<<endl;
    cout<<"Total wait time: "<<twait<<" mins"<<endl;
    cout<<"Average wait time: "<<(double)twait/served<<" mins"<<endl;
    cout<<"Max wait time for any customer: "<<maxwt<<" mins"<<endl;
    cout<<"Total service time: "<<tserv<<" mins"<<endl;
    cout<<"Average service time: "<<(double)tserv/served<<" mins"<<endl;
    cout<<"Max number of customers in line: "<<line1.max<<endl;
    return 0;
}

