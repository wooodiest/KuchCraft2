#include <iostream>

#include "Core/Log.h"

int main()
{
	KuchCraft::Log::Init();
	KuchCraft::Log::Info("Hello KuchCraft{0}", 2);

	return 0;
}
