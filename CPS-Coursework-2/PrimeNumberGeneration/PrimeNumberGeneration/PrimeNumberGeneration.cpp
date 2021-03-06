// PrimeNumberGeneration.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

int num_threads = thread::hardware_concurrency();
mutex mut;

void FindingPrimesByDivisionRevised(ofstream& fileName)
{
	vector<int> storage;
	bool flag = true;
	fileName << 1 << " ";
	fileName << 2 << " ";

	for (int i = 3; i <= 1000000000; i+=2)
	{
		for (int k = 3; k * k <= 1000000000; k+=2)
		{
			if (i % k == 0)
			{
				flag = false;
				break;
			}
			else
			{
				flag = true;
				storage.push_back(i);
				break;
			}
		}
	}
	for (int &i : storage)
	{
		fileName << i << " ";
	}
}

void FindingPrimesBySundaramSieve(int upperBound, ofstream& fileName)
{
	int newUpperBound = (int)sqrt((double)upperBound);
	bool *marked = new bool[upperBound + 1];
	fileName << 2 << " ";
	memset(marked, 0, sizeof(bool) * (upperBound + 1));

	for (int i = 1; i <= newUpperBound; i++)
	{
		for (int j = i; (i + j + 2 * i*j) <= newUpperBound; j++)
		{
			marked[i + j + 2 * i*j] = true;
		}
	}

	for (int i = 1; i <= newUpperBound; i++)
	{
		if (marked[i] == false)
		{
			fileName << 2 * i + 1 << " ";
		}
	}
	delete[] marked;
}

void FindingPrimesByEratosthenesSieve(int upperBound, ofstream& fileName) 
{
	int upperBoundSquareRoot = (int)sqrt((double)upperBound);
	bool *isComposite = new bool[upperBound + 1];
	memset(isComposite, 0, sizeof(bool) * (upperBound + 1));
	for (int m = 2; m <= upperBoundSquareRoot; m++) 
	{
		if (!isComposite[m]) 
		{
			fileName << m << " ";
			for (int k = m * m; k <= upperBound; k += m)
				isComposite[k] = true;
		}
	}
	for (int m = upperBoundSquareRoot; m <= upperBound; m++)
	{
		if (!isComposite[m])
		{
			fileName << m << " ";
		}
	}
	delete[] isComposite;
}

clock_t FindingPrimesByDivisionRevisedParallel(ofstream& fileName)
{
	clock_t t;
	vector<int> storage;
	bool flag = true;
	fileName << 1 << " ";
	fileName << 2 << " ";
	t = clock();
#pragma omp parallel for num_threads(num_threads)
	for (int i = 3; i <= 1000000000; i += 2)
	{
		for (int k = 3; k <= 1000000000; k += 2)
		{
			if (i % k == 0)
			{
				flag = false;
				break;
			}
			else
			{
				flag = true;
				lock_guard<mutex> lock(mut);
				storage.push_back(i);
				break;
			}
		}
	}
	t = clock() - t;
	for (int i : storage)
	{
		fileName << i << " ";
	}
	return t;
}

clock_t FindingPrimesBySundaramSieveParallel(int upperBound, ofstream& fileName)
{
	clock_t t;
	vector<int> storage;
	int newUpperBound = (int)sqrt((double)upperBound);
	bool *marked = new bool[upperBound + 1];
	fileName << 2 << " ";
	memset(marked, 0, sizeof(bool) * (upperBound + 1));
	t = clock();
#pragma omp parallel for num_threads(num_threads) 
	for (int i = 1; i <= newUpperBound; i++)
	{
		for (int j = i; (i + j + 2 * i*j) <= newUpperBound; j++)
		{
			marked[i + j + 2 * i*j] = true;
		}
	}
#pragma omp parallel for num_threads(num_threads) 
	for (int i = 1; i <= newUpperBound; i++)
	{
		if (marked[i] == false)
		{
			lock_guard<mutex> lock(mut);
			storage.push_back(2 * i + 1);
		}
	}
	t = clock() - t;
	for (int i : storage)
	{
		fileName << i << " ";
	}
	delete[] marked;
	return t;
}

clock_t FindingPrimesByEratosthenesSieveParallel(int upperBound, ofstream& fileName)
{
	clock_t t;
	vector<int> storage;
	int upperBoundSquareRoot = (int)sqrt((double)upperBound);
	bool *isComposite = new bool[upperBound + 1];
	memset(isComposite, 0, sizeof(bool) * (upperBound + 1));
	t = clock();
#pragma omp parallel for num_threads(num_threads) 
	for (int m = 2; m <= upperBoundSquareRoot; m++)
	{
		if (!isComposite[m])
		{
			lock_guard<mutex> lock(mut);
			storage.push_back(m);
			for (int k = m * m; k <= upperBound; k += m)
				isComposite[k] = true;
		}
	}
#pragma omp parallel for num_threads(num_threads) 
	for (int m = upperBoundSquareRoot; m <= upperBound; m++)
	{
		if (!isComposite[m])
		{
			lock_guard<mutex> lock(mut);
			storage.push_back(m);
		}
	}
	t = clock() - t;
	for (int i : storage)
	{
		fileName << i << " ";
	}
	delete[] isComposite;
	return t;
}


int main()
{
	ofstream TimeTakenByRevisedBruteForce;
	ofstream TimeTakenBySundaramSieve;
	ofstream TimeTakenByEratosthenesSieve;
	ofstream primeNumbers1;
	ofstream primeNumbers2;
	ofstream primeNumbers3;
	ofstream primeNumbers4;
	TimeTakenByRevisedBruteForce.open("TimeTakenRevisedBruteForce.csv");
	TimeTakenBySundaramSieve.open("TimeTakenForSundramSieve.csv");
	TimeTakenByEratosthenesSieve.open("TimeTakenByEratosthenesSieve.csv");
	clock_t t2;
	clock_t t3;
	clock_t t4;

	
	primeNumbers2.open("PrimeNumbersByDivisionRevised.csv");
	t2 = clock();
	FindingPrimesByDivisionRevised(primeNumbers2);
	t2 = clock() - t2;
	primeNumbers2.close();
	TimeTakenByRevisedBruteForce << "Time Taken in MS for FindingPrimesByDivisionRevised in Sequential" << endl << t2 << endl;
	primeNumbers2.open("PrimeNumbersByDivisionRevised.csv");
	t2 = FindingPrimesByDivisionRevisedParallel(primeNumbers2);
	primeNumbers2.close();
	TimeTakenByRevisedBruteForce << "Time Taken in MS for FindingPrimesByDivisionRevised in Parallel using OpenMP" << endl << t2 << endl;

	
	primeNumbers3.open("PrimeNumbersBySundaramSieve.csv");
	t3 = clock();
	FindingPrimesBySundaramSieve(1000000000, primeNumbers3);
	t3 = clock() - t3;
	primeNumbers3.close();
	TimeTakenByRevisedBruteForce << "Time Taken in MS for FindingPrimesBySundaramSieve in Sequential" << endl << t3 << endl;
	primeNumbers3.open("PrimeNumbersBySundaramSieve.csv");
	t3 = FindingPrimesBySundaramSieveParallel(1000000000, primeNumbers1);
	primeNumbers3.close();
	TimeTakenByRevisedBruteForce << "Time Taken in MS for FindingPrimesBySundaram in Parallel using OpenMP" << endl << t3 << endl;

	
	primeNumbers4.open("PrimeNumbersByEratosthenesSieve.csv");
	t4 = clock();
	FindingPrimesByEratosthenesSieve(1000000000, primeNumbers4);
	t4 = clock() - t4;
	primeNumbers4.close();
	TimeTakenByRevisedBruteForce << "Time Taken in MS for FindingPrimesByEratosthenesSieve in Sequential" << endl << t4 << endl;
	primeNumbers4.open("PrimeNumbersByEratosthenesSieve.csv");
	t4 = FindingPrimesByEratosthenesSieveParallel(1000000000, primeNumbers4);
	primeNumbers4.close();
	TimeTakenByRevisedBruteForce << "Time Taken in MS for FindingPrimesByEratosthenesSieve in Parallel using OpenMP" << endl << t4 << endl;

    return 0;
}


