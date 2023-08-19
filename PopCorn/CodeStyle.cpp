#include "CodeStyle.h"

double CodeStyle::e = 12.3;

const char CodeStyle::f = 'f';

// Code
//
Code::Code()
{

}



// Codestyle
//
CodeStyle::CodeStyle()
	:a(), b(), c(), d(1)
{

}


//
CodeStyle::~CodeStyle()
{

}


//
void CodeStyle::FooVirtual()
{
}


//
void CodeStyle::Foo()
{
}


//
int CodeStyle::FooInt()
{
	return 0;
}


//
double CodeStyle::FooDouble()
{
	return 0.0;
}


//
void CodeStyle::FooStatic()
{
}


//
int CodeStyle::FooStaticInt()
{
	return 0;
}


//
const int CodeStyle::FooStaticConstInt()
{
	return 0;
}
