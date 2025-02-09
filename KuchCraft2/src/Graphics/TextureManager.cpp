#include "kcpch.h"
#include "Graphics/TextureManager.h"

#include "Graphics/Data/Texture2D.h"
#include "Graphics/Data/TextureArray.h"

namespace KuchCraft {

	void TextureManager::Init()
	{
		s_WhiteTexture = std::make_shared<Texture2D>(TextureSpecification{ .Width = 1, .Height = 1 });
		uint32_t whiteColor = 0xffffffff;
		s_WhiteTexture->SetData(&whiteColor, sizeof(whiteColor));
	}

	void TextureManager::Shutdown()
	{

	}

	std::shared_ptr<Texture> TextureManager::Load(const std::filesystem::path& path, const TextureSpecification& specification)
	{	
		if (path.empty())
			return nullptr;

		auto found = s_Data.find(path);
		if (found != s_Data.end())
			return found->second;

		std::shared_ptr<Texture> texture;
		switch (specification.Type)
		{
			case     TextureType::_2D:       texture = std::make_shared<Texture2D>(specification, path);    break;
			case     TextureType::_2D_ARRAY: texture = std::make_shared<TextureArray>(specification, path); break;
			default: Log::Error("[Texture Manager] : Invalid texture type at loading");
		}

		if (texture && texture->IsLoaded())
		{
			s_Data[path] = texture;
			Log::Info("[Texture Manager] : Loaded texture : {}", path.string());
			return texture;
		}
		else
		{
			Log::Info("[Texture Manager] : Failed to load texture : {}", path.string());
			return nullptr;
		}
	}

	void TextureManager::Reload(const std::filesystem::path& path)
	{
		auto found = s_Data.find(path);
		if (found != s_Data.end())
		{
			TextureSpecification specification = found->second->GetSpecification();
			if (specification.Type == TextureType::_2D_ARRAY)
				return;

			std::shared_ptr<Texture> newTexture = Load(path, specification);

			if (newTexture && newTexture->IsLoaded())
			{
				s_Data[path] = newTexture;
				Log::Info("[Texture Manager] : Reloaded texture : {}", path.string());
			}
			else
			{
				Log::Error("[Texture Manager] : Failed to reload texture : {}", path.string());
			}
		}
		else
		{
			Log::Error("[Texture Manager] : Cannot reload, texture not found : {}", path.string());
		}
	}

	void TextureManager::ReloadAll()
	{
		for (auto& [path, texture] : s_Data)
			Reload(path);
	}

}


