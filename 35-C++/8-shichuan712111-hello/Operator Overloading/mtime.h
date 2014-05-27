#ifndef _MTIME_H_
#define _MTIME_H_

#include <iostream>
#include <string>
using namespace std;


class MTime {
private:
	int hour,min,sec;

	int getSecs() const {return ((hour*3600)+(min*60)+sec);}

public:
	MTime();
	MTime(int);

	void setTime(int);

	const MTime operator+(const MTime&);

	bool operator>(const MTime&) const;

	MTime operator++(); // pre-fix
	MTime operator++(int); // post-fix


	friend ostream& operator<<(ostream&, MTime&);

	friend const MTime operator-(const MTime&, const MTime&);
};

#endif

