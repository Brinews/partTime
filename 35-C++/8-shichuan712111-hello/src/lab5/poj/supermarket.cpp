#include <cstdlib>
#include <ctime>

#include <fstream>
#include <iostream>

#include "supermarket.h"

using namespace std;

/***
 * Customer
 */
Customer::Customer(int nCustID, int nStartTime, Random *randor)
{
	nCustomerID = nCustID;
	nArrivalTime = nStartTime + randor->GetArrivalInterval();
	nServiceTime = randor->GetServiceTime();

	/** presumed time **/
	nDequeueTime = nArrivalTime;
}

/***
 * Random
 */
Random::Random()
{
	nServiceTime = SERVTIME;
	nArrivalInterval = ARRIINTV;

	srand(time(0));
}

int Random::GetServiceTime()
{
	return rand()%nServiceTime + 1;
}

int Random::GetArrivalInterval()
{
	return rand()%nArrivalInterval + 1;
}

/***
 * Cashier
 */

/*
Cashier::Cashier()
{
	ptrCurrentCustomer = NULL;
	nServiceTime = 0;
	bIsBusy = false;
}
*/

void Cashier::ServiceCustomer(Customer *cust)
{
	ptrCurrentCustomer = cust;
	nServiceTime += cust->GetServiceTime();

	nEndTime = cust->GetDequeueTime() + cust->GetServiceTime();
	bIsBusy = true;
}

/***
 * Queue
 */
	template <typename T>
void Queue<T>::Enqueue(T *item)
{
	if (IsFull()) {
		cerr << "Queue Is Full." <<endl;
		return;
	}

	cout<<"Customer "<<item->GetCustomerID()<<" arrived at "
		<< item->GetArrivalTime() <<" mins"<<endl;

	ptrArray[tail++] = item;
	nItemCount++;
	tail %= MAXTIME;

	GetMaxCount();
}

	template <typename T>
T* Queue<T>::Dequeue()
{
	if (IsEmpty()) {
		cerr << "Queue Is Empty." << endl;
		return NULL;
	}

	T *item = ptrArray[head++];
	nItemCount--;
	head %= MAXTIME;

	return item;
}

	template <typename T>
Queue<T>::~Queue()
{
	/***
	 * Delete all items
	 */
	while (!IsEmpty()) {
		T *p = Dequeue();
		delete p;
	}
}

int main()
{

	int cnum=1;
	int served=0;
	int t=0;
	int twait=0;
	int maxwt=0;
	int tserv=0;

	Random *myRandor = new Random();
	Cashier *myCashier = new Cashier();
	Customer *cur = new Customer(cnum, t, myRandor);
	Customer *next;
	/***
	 * Change The Random Time Seed Here
	 */
	myRandor->RandomReset();

	cout<<"Simulation started"<<endl;
	t = cur->GetArrivalTime();
	cout<<"Customer 1 arrived at "<< t <<" mins"<<endl;

	myCashier->ServiceCustomer(cur);
	cout<<"Customer 1 began service at "<<t<<" mins"<<endl;

	next = new Customer(++cnum, t, myRandor);
	Queue<Customer> myQueue;

	/* SIMULATION */
	while (t <= MAXTIME)
	{
		if(t == next->GetArrivalTime())
		{
			myQueue.Enqueue(next);
			cnum++;

			next = new Customer(cnum, t, myRandor);
		}

		/***
		 * Get A Cutomer to service
		 */
		if(!myCashier->IsBusy())
		{
			cur = myQueue.Dequeue();

			if(NULL != cur)
			{
				/* Update Dequeue Time */
				cur->SetDequeueTime(t);

				myCashier->ServiceCustomer(cur);
				cout<<"Customer "<< cur->GetCustomerID()<<" began service at "<<t<<" mins"<<endl;
			}
		}

		if(t == myCashier->GetEndTime())
		{
			served++;

			cout<<"Customer "<<cur->GetCustomerID()<<" ended service at "<<t<<" mins"<<endl;

			/** Need destroy */
			delete cur;

			cur = myQueue.Dequeue();
			if(NULL != cur)
			{
				cur->SetDequeueTime(t);
				twait += (t - cur->GetArrivalTime());
				if(maxwt < cur->GetWaitingTime())
					maxwt = cur->GetWaitingTime();

				myCashier->ServiceCustomer(cur);
				cout<<"Customer "<<cur->GetCustomerID()<<" began service at "<<t<<" mins"<<endl;
			}
			else 
			{
				myCashier->SetBusyState(false);
			}
		}

		t++;
	}


	cout<<"Simulation ended"<<endl;
	cout<<"Served "<<served<<" customers"<<endl;
	cout<<"Left in line "<<myQueue.GetItemCount()<<" customers"<<endl;
	cout<<"Total wait time: "<<twait<<" mins"<<endl;
	cout<<"Average wait time: "<<(double)twait/served<<" mins"<<endl;
	cout<<"Max wait time for any customer: "<<maxwt<<" mins"<<endl;
	tserv = myCashier->GetServiceTime();
	cout<<"Total service time: "<<tserv<<" mins"<<endl;
	cout<<"Average service time: "<<(double)tserv/served<<" mins"<<endl;
	cout<<"Max number of customers in line: "<<myQueue.GetMaxCount()<<endl;

	delete cur;
	delete next;
	delete myRandor;
	delete myCashier;

	return 0;
}

