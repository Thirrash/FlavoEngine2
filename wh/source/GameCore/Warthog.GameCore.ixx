module;

#include <stdio.h>

export module Warthog.GameCore;

export namespace Warthog::GameCore
{
	__declspec(dllexport) void lol() { printf("GameCore"); }
}
