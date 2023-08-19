#pragma once

#include<iostream>

class Code;
class CodeStyle;

class Code
{
public:
	Code();
};

class CodeStyle
{
public:
	int a;
	bool b;
	Code c;

	const int d;

	static double e;

	static const char f;


	CodeStyle();
	~CodeStyle();


	virtual void FooAbstract() = 0;
	virtual void FooVirtual();


	void Foo();

	int FooInt();
	double FooDouble();

	static void FooStatic();
	static int FooStaticInt();

	static const int FooStaticConstInt();
};