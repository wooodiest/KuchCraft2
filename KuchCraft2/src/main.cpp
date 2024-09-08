#include "kcpch.h"

#include "Core/Log.h"
#include "Core/Config.h"

int main()
{
	KuchCraft::ApplicationConfig::Init();

	KuchCraft::Log::Init();
	KuchCraft::Log::Info("Hello KuchCraft");

	return 0;
}
