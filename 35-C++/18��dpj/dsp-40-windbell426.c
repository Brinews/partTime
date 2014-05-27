#include <math.h>

double GetY(double x, double a, double b, double alpha, double beta, double gamma)
{
	double t = b * sin(alpha + gamma) / (a * sin(gamma + beta));

	if (t != 0)
		return x / t;

	return 0;
}

int main()
{
	double a = GetY(0.5, 0.1, 0.3, 0.3, 0.4, 0.5);
	printf("%lf\n", a);

	return 0;
}
