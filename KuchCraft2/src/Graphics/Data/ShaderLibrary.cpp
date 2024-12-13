///
/// @file ShaderLibrary.cpp
/// 
/// @author Michal Kuchnicki
///

#include "kcpch.h"
#include "Graphics/Data/ShaderLibrary.h"

namespace KuchCraft {

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::filesystem::path& filePath)
	{
		auto shader = std::make_shared<Shader>(filePath);
		Add(shader);

		return shader;
	}

	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		const auto& name = shader->GetName();
		s_Shaders[name] = shader;

		Log::Info("[Shader Library] : Loaded shader : {}", name);
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		if (Exists(name))
			return s_Shaders[name];

		Log::Error("[Shader Library] : Shader {} : Not found", name);
		return nullptr;
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return s_Shaders.find(name) != s_Shaders.end();
	}

	void ShaderLibrary::ReCompile(const std::string& name)
	{
		if (Exists(name))
		{
			s_Shaders[name]->ReCompile();
			Log::Info("[Shader Library] : Recompiled shader : {}", name);
		}
		else
			Log::Error("[Shader Library] : Shader '{}' : Does not exist", name);
	}

	void ShaderLibrary::ReCompileAll()
	{
		for (auto& [name, shader] : s_Shaders)
		{
			shader->ReCompile();
			Log::Info("[Shader Library] : Recompiled shader : {}", name);
		}
	}

	void ShaderLibrary::AddSubstitution(const std::pair<std::string, std::string>& sub)
	{
		const std::string& from = sub.first;
		const std::string& to   = sub.second;

		s_Substitutions["##" + from] = to;
	}

}


