///
/// @file RendererData.h
/// 
/// @author Michal Kuchnicki
/// 

#pragma once

#include "Graphics/Data/Shader.h"

namespace KuchCraft {

	/// Main Renderer Data
	struct RendererData
	{
		bool Logs = true;
	};

	/// TMP
	struct RendererTMPData
	{
		std::shared_ptr<Shader> Shader;
		uint32_t VAO = 0;
	};

}