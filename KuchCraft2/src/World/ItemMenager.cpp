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

			s_Data[ID] = ItemInfo{};
			ItemInfo& info = s_Data[ID];

			///// === BASIC PROPERTIES 
			if (item.contains("name"))
				info.Name = item["name"];

			s_NameData[info.Name] = ID;

			if (item.contains("description"))
				info.Description = item["description"];

			if (item.contains("type"))
				info.Type = StringToItemType(item["type"].get<std::string>());

			if (item.contains("transparent"))
				info.Transparent = item["transparent"];

			if (item.contains("stackSize"))
				info.StackSize = item["stackSize"].get<int>();

			if (item.contains("recipes")) // TODO: Add crafting recipes and if size > 0 set IsCraftable to true
				info.IsCraftable = true;

			///// === DURABILITY & BREAKING ===
			if (item.contains("durability"))
				info.Durability = item["durability"].get<int>();

			if (item.contains("breakingTime"))
				info.BreakingTime = item["breakingTime"].get<float>();		

			///// === DEFENSE & DAMAGE ===
			if (item.contains("defense"))
				info.Defense = item["defense"].get<int>();

			if (item.contains("attackDamage"))
				info.AttackDamage = item["attackDamage"].get<int>();

			if (item.contains("attackSpeed"))
				info.AttackSpeed = item["attackSpeed"].get<float>();

			///// === FOOD PROPERTIES ===
			if (item.contains("foodValue"))
			{
				info.IsEdible = true;
				info.FoodValue = item["foodValue"].get<int>();
			}

			if (item.contains("foodEffects"))
			{
				for (const auto& effect : item["foodEffects"])
				{
					std::string name = effect["name"];
					float value = effect["value"];
					info.FoodEffects.emplace_back(name, value);
				}

				if (info.FoodEffects.size() > 0)
					info.HasSpecialEffect = true;
			}
			///// === ENVIRONMENTAL PROPERTIES ===
			if (item.contains("lightEmission"))
				info.LightEmission = item["lightEmission"].get<float>();

			if (item.contains("lightColor"))
			{
				auto color = item["lightColor"];
				info.LightColor = { color[0].get<float>(), color[1].get<float>(), color[2].get<float>(), color[3].get<float>() };
			}

			///// === PHYSICAL PROPERTIES ===
			if (item.contains("weight"))
				info.Weight = item["weight"].get<float>();

			if (item.contains("friction"))
				info.Friction = item["friction"].get<float>();

			if (item.contains("textures") || item.contains("texture"))
			{
				std::string textures[block_face_count];
				if (item.contains("textures"))
				{
					auto tex = item["textures"];

					std::string defaultAll  = tex.contains("all")  ? tex["all"] .get<std::string>() : "";
					std::string defaultSide = tex.contains("side") ? tex["side"].get<std::string>() : defaultAll;

					textures[(int)BlockFaces::Front]  = tex.contains("front")  ? tex["front"] .get<std::string>() : defaultSide;
					textures[(int)BlockFaces::Left]   = tex.contains("left")   ? tex["left"]  .get<std::string>() : defaultSide;
					textures[(int)BlockFaces::Back]   = tex.contains("back")   ? tex["back"]  .get<std::string>() : defaultSide;
					textures[(int)BlockFaces::Right]  = tex.contains("right")  ? tex["right"] .get<std::string>() : defaultSide;
					textures[(int)BlockFaces::Top]    = tex.contains("top")    ? tex["top"]   .get<std::string>() : defaultAll;
					textures[(int)BlockFaces::Bottom] = tex.contains("bottom") ? tex["bottom"].get<std::string>() : defaultAll;
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

					for (int y = 0; y < (int)blockTextureSize; y++)
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
				texture2D->SetData(mergedData, (uint32_t)size);
				TextureManager::Add(texture2D, info.Name);

				delete[] mergedData;
				s_ItemTextureArrayLayers[ID] = layerIndex;
				layerIndex++;
			}
			
		}

		for (const auto& item : json["Items"])
		{
			ItemID ID = item["id"];
			ItemInfo& info = s_Data[ID];

			///// === DURABILITY & BREAKING ===
			if (item.contains("breakableBy"))
			{
				for (const auto& tool : item["breakableBy"])
				{
					if (tool.is_number_integer())
						info.BreakableBy.insert(tool.get<int>());
					else if (tool.is_string())
					{
						ItemID toolID = GetItemIDByName(tool.get<std::string>());
						if (toolID != 0)
							info.BreakableBy.insert(toolID);
					}
				}
			}

			if (item.contains("drops"))
			{
				for (const auto& drop : item["drops"])
				{
					ItemID dropItemID = 0;
					int dropCount = 1;

					if (drop.contains("item"))
					{
						if (drop["item"].is_number_integer())
							dropItemID = drop["item"].get<int>();
						else if (drop["item"].is_string())
							dropItemID = GetItemIDByName(drop["item"].get<std::string>());
					}

					if (drop.contains("count"))
						dropCount = drop["count"].get<int>();

					if (dropItemID != 0)
						info.Drops.emplace_back(dropItemID, dropCount);
				}
			}
			else
				info.Drops.emplace_back(ID, 1);
		}

	}

}

