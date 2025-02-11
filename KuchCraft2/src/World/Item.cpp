#include "kcpch.h"
#include "Item.h"

#include "World/ItemMenager.h"
#include "Graphics/TextureManager.h"

namespace KuchCraft {

	Item::Item()
	{
	}

	Item::Item(ItemID id)
		: m_ID(id)
	{
	}

	Item::Item(ItemData data)
		: m_ID((ItemID)data)
	{
	}

	Item::~Item()
	{
	}

	void Item::Use()
	{
		const auto& info = GetInfo();

		switch (info.Type)
		{
			case ItemType::Block: {

				break;
			}
			case ItemType::Food: {

				break;
			}
			case ItemType::Tool: {

				break;
			}
			case ItemType::Utility: {

				break;
			}
		}
	}

	const ItemInfo& Item::GetInfo() const
	{
		return ItemMenager::GetInfo(m_ID);
	}

	const std::shared_ptr<Texture>& Item::GetTexture(ItemID id)
	{
		return TextureManager::Get(ItemMenager::GetInfo(id).Name);
	}
}
