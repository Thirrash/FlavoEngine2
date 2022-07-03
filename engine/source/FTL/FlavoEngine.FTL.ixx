module;

#include <stdio.h>

export module FlavoEngine.FTL;

export namespace FlavoEngine::FTL
{
	struct Test
	{
		int x, y;
	};

	void fun() { printf("FTL lib"); }
}
