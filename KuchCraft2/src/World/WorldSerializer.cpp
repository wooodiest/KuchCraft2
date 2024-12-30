#include "kcpch.h"
#include "World/WorldSerializer.h"

#include "World/Entity.h"
#include "World/NativeScripts.h"
#include "Graphics/TextureManager.h"
#include "Core/Config.h"

#include <json.hpp>

namespace KuchCraft {

	/// Converts an ImageFilter enum to a string representation.
	/// @param filter - the ImageFilter enum to convert.
	/// @return A string representing the filter.
	static std::string ImageFilterToString(ImageFilter filter)
	{
		switch (filter)
		{
			case ImageFilter::None:    return "None";
			case ImageFilter::LINEAR:  return "Linear";
			case ImageFilter::NEAREST: return "Nearest";
			default: return "None";
		}
	}

	/// Converts a string to an ImageFilter enum.
    /// @param filter - the string representation of the filter.
    /// @return The corresponding ImageFilter enum.
	static ImageFilter StringToImageFilter(const std::string& filter)
	{
		if (filter == "Linear")
			return ImageFilter::LINEAR;
		else if (filter == "Nearest")
			return ImageFilter::NEAREST;

		return ImageFilter::None;
	}

	/// @brief Converts a TextureType enum to a string representation.
    /// @param type - the TextureType enum to convert.
    /// @return A string representing the texture type.
	static std::string TextureTypeToString(TextureType type)
	{
		switch (type)
		{
			case TextureType::None: return "None";
			case TextureType::_2D:  return "2D";
			default: return "None";
		}
	}

	/// Converts a string to a TextureType enum.
	/// @param type - the string representation of the texture type.
	/// @return The corresponding TextureType enum.
	static TextureType StringToTextureType(const std::string& type)
	{
		if (type == "2D")
			return TextureType::_2D;

		return TextureType::None;
	}

	WorldSerializer::WorldSerializer(World* world)
		: m_World(world)
	{

	}

	WorldSerializer::~WorldSerializer()
	{

	}

	bool WorldSerializer::Serialize()
	{
		if (!m_World)
		{
			Log::Error("[World Serializer] : Invalid World");
			return false;
		}

		if (m_World->GetPath().empty())
		{
			Log::Error("[World Serializer] : Invalid path");
			return false;
		}

		nlohmann::json wjson;
		for (auto handle : m_World->m_Registry.view<entt::entity>())
		{
			Entity entity = { handle, &(*m_World) };
			if (!entity)
				continue;

			nlohmann::json ejson;

			if (entity.HasComponent<IDComponent>())
			{
				ejson["UUID"] = static_cast<uint64_t>(entity.GetComponent<IDComponent>().ID);
			}

			if (entity.HasComponent<TagComponent>()) 
			{
				ejson["Tag"] = entity.GetComponent<TagComponent>().Tag;
			}

			if (entity.HasComponent<TransformComponent>()) 
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				ejson["Transform"] = {
					{ "Translation", { transform.Translation.x, transform.Translation.y, transform.Translation.z }},
					{ "Rotation",    { transform.Rotation.x,    transform.Rotation.y,    transform.Rotation.z    }},
					{ "Scale",       { transform.Scale.x,       transform.Scale.y,       transform.Scale.z       }}
				};
			}

			if (entity.HasComponent<NativeScriptComponent>())
			{
				auto& script = entity.GetComponent<NativeScriptComponent>();
				ejson["NativeScript"] = {
					{ "ScriptName", script.ScriptName },
					{ "State", script.Instance->Serialize() }
				};
			}

			if (entity.HasComponent<CameraComponent>())
			{
				auto& camera = entity.GetComponent<CameraComponent>();

				ejson["Camera"] = {
					{ "Primary",               camera.Primary                 },
					{ "FixedAspectRatio",      camera.FixedAspectRatio        },
					{ "UseTransformComponent", camera.UseTransformComponent   },
					{ "AspectRatio",		   camera.Camera.GetAspectRatio() },
					{ "NearClip",			   camera.Camera.GetNearClip()    },
					{ "FarClip",			   camera.Camera.GetFarClip()     },
				};
			}

			if (entity.HasComponent<Sprite2DRendererComponent>())
			{
				auto& sprite = entity.GetComponent<Sprite2DRendererComponent>();
				ejson["Sprite2DRenderer"] = {
					{ "Color", {      sprite.Color.r, sprite.Color.g, sprite.Color.b, sprite.Color.a }},
					{ "Texture",      sprite.Texture ? sprite.Texture->GetPath() : ""                 },
					{ "Type",         TextureTypeToString(sprite.Texture ? sprite.Texture->GetSpecification().Type   : TextureType::None) },
					{ "ImageFilter",  ImageFilterToString(sprite.Texture ? sprite.Texture->GetSpecification().Filter : ImageFilter::None) },
					{ "GenerateMips", sprite.Texture ? sprite.Texture->GetSpecification().GenerateMips : false },
				};
			}

			if (entity.HasComponent<Sprite3DRendererComponent>())
			{
				auto& sprite = entity.GetComponent<Sprite3DRendererComponent>();
				ejson["Sprite3DRenderer"] = {
					{ "Color", {      sprite.Color.r, sprite.Color.g, sprite.Color.b, sprite.Color.a }},
					{ "Texture",      sprite.Texture ? sprite.Texture->GetPath() : ""                 },
					{ "Type",         TextureTypeToString(sprite.Texture ? sprite.Texture->GetSpecification().Type   : TextureType::None) },
					{ "ImageFilter",  ImageFilterToString(sprite.Texture ? sprite.Texture->GetSpecification().Filter : ImageFilter::None) },
					{ "GenerateMips", sprite.Texture ? sprite.Texture->GetSpecification().GenerateMips : false },
				};
			}

			wjson["Entities"].push_back(ejson);
		}

		std::ofstream file(m_World->GetPath() / ApplicationConfig::GetWorldData().WorldDataFile);
		if (!file.is_open())
		{
			Log::Error("[World Serializer] : Failed to open : {}" , m_World->GetPath().string());
			return false;
		}

		file << wjson.dump(4, ' ', false, nlohmann::json::error_handler_t::strict);
		file.close();

		Log::Info("[World Serializer] : Serialized : {}", m_World->GetPath().string());
		return true;
	}

	bool WorldSerializer::SerializeRuntime()
	{
		/// TODO
		return false;
	}

	bool WorldSerializer::Deserialize()
	{
		if (!m_World)
		{
			Log::Error("[World Serializer] : Invalid World");
			return false;
		}

		std::ifstream file(m_World->GetPath() / ApplicationConfig::GetWorldData().WorldDataFile);
		if (!file.is_open())
		{
			Log::Error("[World Serializer] : Failed to open : {}", m_World->GetPath().string());
			return false;
		}

		for (auto handle : m_World->m_Registry.view<entt::entity>())
		{
			Entity entity = { handle, &(*m_World) };
			if (!entity)
				continue;

			m_World->DestroyEntity(entity);
		}

		nlohmann::json wjson;
		file >> wjson;
		file.close();

		for (auto& ejson : wjson["Entities"])
		{
			if (!ejson.contains("UUID") || !ejson.contains("Tag"))
				continue;

			Entity entity = m_World->CreateEntityWithUUID(
				UUID(ejson["UUID"].get<uint64_t>()),
				ejson["Tag"].get<std::string>()
			);

			if (ejson.contains("NativeScript"))
			{
				const auto& scriptName = ejson["NativeScript"]["ScriptName"];
				if (!scriptName.empty())
				{
					auto& nsc = entity.AddComponent<NativeScriptComponent>();
					bool scriptFound = IterateComponentGroup(AllNativeScripts{}, scriptName, [&nsc](auto scriptType) {
						using Script = decltype(scriptType);
						nsc.Bind<Script>();
					});

					if (scriptFound)
					{
						if (ejson["NativeScript"].contains("State") && !ejson["NativeScript"]["State"].is_null())
						{
							nsc.Instance = nsc.InstantiateScript();
							nsc.Instance->m_Entity = entity;
							nsc.Instance->Deserialize(ejson["NativeScript"]["State"]);
							nsc.Instance->OnCreate();
						}
					}
					else
						Log::Error("[World Serializer] : Script not found : {}", std::string(scriptName));		
				}
			}

			if (ejson.contains("Transform"))
			{
				auto& transform = entity.HasComponent<TransformComponent>() ? entity.GetComponent<TransformComponent>() : entity.AddComponent<TransformComponent>();
				const auto& translation = ejson["Transform"]["Translation"];
				const auto& rotation    = ejson["Transform"]["Rotation"];
				const auto& scale       = ejson["Transform"]["Scale"];

				transform.Translation = { translation[0], translation[1], translation[2] };
				transform.Rotation    = { rotation[0],    rotation[1],    rotation[2]    };
				transform.Scale       = { scale[0],       scale[1],       scale[2]       };
			}

			if (ejson.contains("Camera"))
			{
				auto& camera = entity.HasComponent<CameraComponent>() ? entity.GetComponent<CameraComponent>() : entity.AddComponent<CameraComponent>();
				const auto& primary               = ejson["Camera"]["Primary"];
				const auto& fixedAspectRatio      = ejson["Camera"]["FixedAspectRatio"];
				const auto& useTransformComponent = ejson["Camera"]["UseTransformComponent"];
				const auto& aspectRatio           = ejson["Camera"]["AspectRatio"];
				const auto& nearClip              = ejson["Camera"]["NearClip"];
				const auto& farClip               = ejson["Camera"]["FarClip"];

				camera.Primary               = primary;
				camera.FixedAspectRatio      = fixedAspectRatio;
				camera.UseTransformComponent = useTransformComponent;
				camera.Camera.SetAspectRatio(aspectRatio);
				camera.Camera.SetNearClip(nearClip);
				camera.Camera.SetFarClip(farClip);
			}

			if (ejson.contains("Sprite2DRenderer"))
			{
				auto& sprite = entity.HasComponent<Sprite2DRendererComponent>() ? entity.GetComponent<Sprite2DRendererComponent>() : entity.AddComponent<Sprite2DRendererComponent>();
				const auto& color  = ejson["Sprite2DRenderer"]["Color"];
				const auto& type   = ejson["Sprite2DRenderer"]["Type"];
				const auto& filter = ejson["Sprite2DRenderer"]["ImageFilter"];
				const auto& mips   = ejson["Sprite2DRenderer"]["GenerateMips"];

				TextureSpecification spec;
				spec.Type         = StringToTextureType(type);
				spec.Filter       = StringToImageFilter(filter);
				spec.GenerateMips = mips;

				sprite.Color   = { color[0], color[1], color[2], color[3] };
				sprite.Texture = TextureManager::Load(std::filesystem::path(std::string(ejson["Sprite2DRenderer"]["Texture"])), spec);
			}

			if (ejson.contains("Sprite3DRenderer"))
			{
				auto& sprite = entity.HasComponent<Sprite3DRendererComponent>() ? entity.GetComponent<Sprite3DRendererComponent>() : entity.AddComponent<Sprite3DRendererComponent>();
				const auto& color  = ejson["Sprite3DRenderer"]["Color"];
				const auto& type   = ejson["Sprite3DRenderer"]["Type"];
				const auto& filter = ejson["Sprite3DRenderer"]["ImageFilter"];
				const auto& mips   = ejson["Sprite3DRenderer"]["GenerateMips"];

				TextureSpecification spec;
				spec.Type         = StringToTextureType(type);
				spec.Filter       = StringToImageFilter(filter);
				spec.GenerateMips = mips;

				sprite.Color   = { color[0], color[1], color[2], color[3] };
				sprite.Texture = TextureManager::Load(std::filesystem::path(std::string(ejson["Sprite3DRenderer"]["Texture"])), spec);
			}
		}

		Log::Info("[World Serializer] : Deserialized : {}", m_World->GetPath().string());
		return true;
	}

	bool WorldSerializer::DeserializeRuntime()
	{
		/// TODO
		return false;
	}

}
