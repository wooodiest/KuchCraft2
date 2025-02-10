#include "kcpch.h"
#include "ItemMenager.h"

#include "Core/Config.h"

#include "World/ItemData.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Data/Texture2D.h"

#include <json.hpp>
#include <stb_image.h>

namespace KuchCraft {

	void ItemMenager::Reload()
	{
		std::ifstream file(ApplicationConfig::GetWorldData().TexturePackFile);
		if (!file.is_open())
		{
			Log::Error("[ItemMenager] : Failed to open : {}", ApplicationConfig::GetWorldData().TexturePackFile);
			return;
		}
		nlohmann::json json;
		file >> json;
		file.close();

		uint32_t blockTextureSize = ApplicationConfig::GetRendererData().BlockTextureSize;
		uint32_t itemCount        = 0;

		for (const auto& item : json["Items"])
			if (item.contains("id") && item["id"].get<int>())
				itemCount++;

		TextureSpecification spec;
		spec.Type    = TextureType::_2D_ARRAY;
		spec.Filter  = ImageFilter::NEAREST;
		spec.Format  = ImageFormat::RGBA8;
		spec.Width   = blockTextureSize * block_face_count;
		spec.Height  = blockTextureSize;
		spec.Layers  = itemCount;
		s_ItemTextureArray = std::make_shared<TextureArray>(spec, ApplicationConfig::GetWorldData().TexturePackFile);
		TextureManager::Add(s_ItemTextureArray, ApplicationConfig::GetWorldData().TexturePackFile);

		uint32_t channelCount = Texture::ImageFormatToChannelCount(spec.Format);
		uint32_t layerIndex   = 0;

		for (const auto& item : json["Items"])
		{
			if (!item.contains("id"))
			{
				Log::Error("[ItemMenager] : Item needs id and name");
				return;
			}

			ItemID ID = item["id"];
			ItemInfo info;

			if (item.contains("name"))
				info.Name = item["name"];

			if (item.contains("isBlock"))
				info.IsBlock = item["isBlock"];

			if (item.contains("textures") || item.contains("texture"))
			{
				std::string textures[block_face_count];
				if (item.contains("textures"))
				{
					auto tex = item["textures"];

					std::string defaultAll  = tex.contains("all")  ? tex["all"] .get<std::string>() : "";
					std::string defaultSide = tex.contains("side") ? tex["side"].get<std::string>() : defaultAll;

					textures[0] = tex.contains("front")  ? tex["front"] .get<std::string>() : defaultSide;
					textures[1] = tex.contains("left")   ? tex["left"]  .get<std::string>() : defaultSide;
					textures[2] = tex.contains("back")   ? tex["back"]  .get<std::string>() : defaultSide;
					textures[3] = tex.contains("right")  ? tex["right"] .get<std::string>() : defaultSide;
					textures[4] = tex.contains("top")    ? tex["top"]   .get<std::string>() : defaultAll;
					textures[5] = tex.contains("bottom") ? tex["bottom"].get<std::string>() : defaultAll;
				}
				else
				{
					std::string path = item["texture"].get<std::string>();
					for (int i = 0; i < block_face_count; i++)
						textures[i] = path;
				}

				for (auto& path : textures)
				{
					std::filesystem::path fsPath(path);
					if (fsPath.is_relative())
						path = ApplicationConfig::GetWorldData().TexturesDirectory + "/" + path;
				}

				size_t size = spec.Width * spec.Height * channelCount;
				uint8_t* mergedData = new uint8_t[size];

				for (int i = 0; i < block_face_count; i++)
				{
					int width, height, channels;
					stbi_set_flip_vertically_on_load(1);
					stbi_uc* data = stbi_load(textures[i].c_str(), &width, &height, &channels, channelCount);
					if (!data)
					{
						Log::Error("[ItemMenager] : Failed to load texture {}", textures[i]);
						return;
					}

					for (int y = 0; y < blockTextureSize; y++)
					{
						std::memcpy(
							mergedData + (y * spec.Width * channelCount) + (i * blockTextureSize * channelCount),
							data + (y * blockTextureSize * channelCount),
							blockTextureSize * channelCount);
					}
					stbi_image_free(data);
				}

				s_ItemTextureArray->SetLayerData(layerIndex, mergedData);

				auto texture2D = std::make_shared<Texture2D>(TextureSpecification{ .Width = s_ItemTextureArray->GetWidth(), .Height = s_ItemTextureArray->GetHeight(), .Filter = ImageFilter::NEAREST });
				texture2D->SetData(mergedData, size);
				TextureManager::Add(texture2D, info.Name);

				delete[] mergedData;
				s_ItemTextureArrayLayers[ID] = layerIndex;
				layerIndex++;
			}
			
		}
	}

}

