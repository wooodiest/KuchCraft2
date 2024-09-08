#include <iostream>

#include "Core/Log.h"
#include "Core/Config.h"

int main()
{
	KuchCraft::ApplicationConfig::Init();
	KuchCraft::Log::Init();
	KuchCraft::Log::Info("Hello KuchCraft{0}", 2);

	return 0;
}
