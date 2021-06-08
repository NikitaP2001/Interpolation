#pragma once
#include <bitset>
#include <chrono>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
int htoi(char arr[]);
void squeeze(char s1[], char s2[]);
int any(char s1[], char s2[]);
unsigned int getbits(unsigned x, int p, int n);
int setbits(unsigned int x, int p, int n, int y);
int invert(unsigned int x, int p, int n);
int rightrot(unsigned int x, int n);
int bitcount2(unsigned int x);
int bitcount1(unsigned int x);
void lower2(char str[]);
int binsearchv2(int x, int v[], int n);
int binsearch(int x, int v[], int n);
void escape(char s[], char t[]);
void revescape(char s[], char t[]);
void expand(char s1[], char s2[]);
void itoa(int n, char s[]);
void itob(int n, char s[], int d);
void itoam(int n, char s[], int min);
double atof(char* s, int &i);
bool issymbol(char sym);