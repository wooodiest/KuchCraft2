#include "kcpch.h"
#include "Item.h"

#include "World/Item/ItemMenager.h"
#include "Graphics/TextureManager.h"

namespace KuchCraft {

	Item::Item()
	{
	}

	Item::Item(ItemID id)
		: m_ID(id)
	{
	}

	Item::Item(const std::string& name)
		: m_ID(ItemMenager::GetItemIDByName(name))
	{
	}

	Item::Item(ItemData data)
		: m_ID((ItemID)data)
	{
	}

	Item::Item(ItemData data, ItemRotation rotation)
		: m_ID((ItemID)data)
	{
		SetRotation(rotation);
	}

	Item::Item(ItemID id, ItemRotation rotation)
		: m_ID(id)
	{
		SetRotation(rotation);
	}

	Item::Item(const std::string& name, ItemRotation rotation)
		: m_ID(ItemMenager::GetItemIDByName(name))
	{
		SetRotation(rotation);
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
