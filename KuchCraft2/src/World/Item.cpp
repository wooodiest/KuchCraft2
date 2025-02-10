#include "kcpch.h"
#include "Item.h"

#include "World/ItemMenager.h"
#include "Graphics/TextureManager.h"

namespace KuchCraft {

	const std::shared_ptr<Texture>& Item::GetTexture(ItemID id)
	{
		return TextureManager::Get(ItemMenager::GetInfo(id).Name);
	}
}
