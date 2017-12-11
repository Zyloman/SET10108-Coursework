// PrimeNumberGeneration.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

void FindingPrimesByDivision()
{
	bool flag = true;
	for (int i = 2; i <= 100000000000; i++)
	{
		for (int k = 2; k <= 100000000000; k++)
		{
			if (i == k)
			{
				continue;
			}
			else if (i % k == 0)
			{
				flag = false;
				break;
			}
			else
			{
				flag = true;
			}
		}

		if (flag == true)
		{
			cout << i << endl;
		}
	}
}

int main()
{
	clock_t t;
	t = clock();
	FindingPrimesByDivision;
	t = clock() - t;
	cout << "Time Taken in MS" << t << endl;
    return 0;
}


