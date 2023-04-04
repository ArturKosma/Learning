#include "pch.h"
#include "ii_threadLocal.h"

thread_local unsigned ii_threadLocal::mynum = 666;

ii_threadLocal::ii_threadLocal()
{
}


ii_threadLocal::~ii_threadLocal()
{
}

void ii_threadLocal::operator()(unsigned InSomeNum)
{
	mynum = InSomeNum;

	printf("%d\n", mynum);
}

unsigned ii_threadLocal::print()
{
	printf("%d\n", mynum);

	return 0;
}

