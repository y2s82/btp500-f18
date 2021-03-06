﻿// Recursion.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <windows.h>

unsigned long int factorial(int n);

int main()
{
	int n;
	unsigned long int result;
	printf("Enter the factorial integer: ");
	scanf("%d", &n);
	if (n < 0) {
		printf("Invalid value, exiting...\n");
		return -1;
	}
	FILE *fp = fopen("Memory.txt", "w");
	fclose(fp);
	result = factorial(n);
	printf("The factorial of %d is %lu\n", n, result);
	Sleep(3000);
    return 0;
}

// n! = n*(n-1)*(n-2)*(n-3)...*2*1
// 0! = 1
// n! = n*(n-1)!
unsigned long int factorial(int n) {
	unsigned long int result;
	FILE *fp = fopen("Memory.txt", "a");
	fprintf(fp, "factorial(n:%2d): result is in memory address %p\n", n, &result);
	fclose(fp);
	if (n==0) {
		result = 1;//The base case
	}
	else {
		result = n * factorial(n - 1);//The recursive case
	}
	fp = fopen("Memory.txt", "a");
	fprintf(fp, "factorial(n:%2d): result is in memory address %p\n", n, &result);
	fprintf(fp, "factorial(n:%2d): result is %d\n", n, result);
	fclose(fp);
	return result;
}

