#include "mtime.h"

MTime::MTime() {
	setTime(0);
}

MTime::MTime(int s) {
	setTime(s);
}

void MTime::setTime(int s) {
	hour = s/3600;
	min = (s/60)%60;
	sec = s%60;
}

const MTime MTime::operator+(const MTime &m) {
	int secs = (hour*3600)+(m.hour*3600);
	secs += (min*60) + (m.min*60);
	secs += sec + m.sec;
	MTime temp(secs);
	return temp;
}

bool MTime::operator>(const MTime &m) const {
	if (getSecs()>m.getSecs()) return true;
	else return false;
}

MTime MTime::operator++() {
	setTime(getSecs()+1);
	return *this;
}

MTime MTime::operator++(int i) {
	MTime temp(getSecs());
	setTime(getSecs()+1);
	return temp;
}

const MTime operator-(const MTime &m, const MTime &n) {
	int secs = (m.hour*3600)-(n.hour*3600);
	secs += (m.min*60) - (n.min*60);
	secs += m.sec - n.sec;
	MTime temp(secs);
	return temp;
}


ostream &operator<<(ostream &out, MTime &m) {
	if (m.hour<10) out << 0;
	out << m.hour << ":";
	if (m.min<10) out << 0;
	out << m.min << ":";
	if (m.sec<10) out << 0;
	out << m.sec;
	return out;
}

