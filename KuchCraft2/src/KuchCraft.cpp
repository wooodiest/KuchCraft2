///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "KuchCraft.h"
#include "World/Entity.h"

namespace KuchCraft {

	KuchCraft::KuchCraft()
	{
		/// tmp, future in menu can pick world to load
		m_World = std::make_shared<World>();
		/// Load entities
		auto e1 = m_World->CreateEntity("Michal");
		auto e2 = m_World->CreateEntity("Marysia");
		auto e3 = m_World->CreateEntity("Piotr");

	}

	KuchCraft::~KuchCraft()
	{

	}

	void KuchCraft::OnUpdate(float dt)
	{
		if (m_World)
			m_World->OnUpdate(dt);
	}

	void KuchCraft::OnEvent(Event& e)
	{
		if (m_World)
			m_World->OnEvent(e);
	}

	void KuchCraft::OnTick()
	{

	}

}
