#ifndef _SUPERMARKET_H

#define MAXTIME	720
#define SERVTIME 4
#define ARRIINTV 4

class Random {
private:
	int nServiceTime;
	int nArrivalInterval;

public:
	Random();

	void RandomReset(int nST = 4, int nAI = 4)
	{
		nServiceTime = nST;
		nArrivalInterval = nAI;
	}

	~Random()
	{
	}

	int GetServiceTime();
	int GetArrivalInterval();
};

class Customer {
private:
	int nCustomerID;
	int nArrivalTime;
	int nServiceTime;
	int nDequeueTime;

public:
	Customer(int nCustID, int nStartTime, Random *randor);

	int GetCustomerID()
	{
		return nCustomerID;
	}

	int GetArrivalTime()
	{
		return nArrivalTime;
	}

	int GetServiceTime()
	{
		return nServiceTime;
	}

	int GetDequeueTime()
	{
		return nDequeueTime;
	}

	int GetWaitingTime()
	{
		return nDequeueTime - nArrivalTime;
	}

	void SetDequeueTime(int nDT)
	{
		nDequeueTime = nDT;
	}

	~Customer(){}
};

template <typename T>
class Queue {
private:
	T *ptrArray[MAXTIME];
	int head, tail;

	int nItemCount;
	int nMaxCount;
private:
	bool IsFull()
	{
		return nItemCount == MAXTIME;
	}

	bool IsEmpty()
	{
		return nItemCount == 0;
	}

public:
	Queue():head(0),tail(0),nItemCount(0),nMaxCount(0)
	{
	}

	~Queue();

	void Enqueue(T* item);
	T* Dequeue();

	int GetItemCount()
	{
		return nItemCount;
	}

	int GetMaxCount()
	{
		if (nItemCount > nMaxCount)
			nMaxCount = nItemCount;

		return nMaxCount;
	}
};

class Cashier {
private:
	Customer *ptrCurrentCustomer;

	/** Total service Time */
	int nServiceTime;

	/** Service One Customer End */
	int nEndTime;
	bool bIsBusy;

public:
	Cashier()
	{
		nServiceTime = 0;
		bIsBusy = false;
	}

	~Cashier()
	{
	}

	void SetBusyState(bool s)
	{
		bIsBusy = s;
	}

	bool IsBusy()
	{
		return bIsBusy;
	}

	int GetEndTime()
	{
		return nEndTime;
	}

	int GetServiceTime()
	{
		return nServiceTime;
	}

	void ServiceCustomer(Customer *cust);

};

#endif
