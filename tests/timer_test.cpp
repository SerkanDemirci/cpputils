#include <iostream>
#include <cassert>
#include "../src/timer.hpp"

unsigned long int bar(unsigned long int n)
{
    assert(n >= 0);
    unsigned long int a = 1;
    for (unsigned long int k = 0; k < n; k++)
    {
        a = a ^ k + 23;
    }
    return a;
}

void foo(long unsigned int ms)
{
    long double c = (long double)CLOCKS_PER_SEC;
    unsigned long int n = (ms / 1.15) * c / 2;

    long unsigned int a = bar(n);
}

int main()
{
    Timer<WallClock> wall;
    Timer<HighResolutionClock> hr;
    Timer<CpuClock> ct;

    ct.start();
    hr.start();
    wall.start();

    foo(1000);

    hr.stop();
    wall.stop();
    ct.stop();
    assert(hr.seconds<double>() - hr.milliseconds<double>() / 1000 <= 10e-5);

    std::cout.precision(10);
    std::cout << "Wall Clock : \t\t" << wall.seconds<double>() << " seconds" <<  std::endl;
    std::cout << "High Resolution Clock : " << hr.seconds<double>() << " seconds" << std::endl;
    std::cout << "CPU clock : \t\t" << ct.seconds<double>() << " seconds" << std::endl;
	return 0;
}
