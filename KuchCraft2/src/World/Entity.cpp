#include "kcpch.h"
#include "World/Entity.h"

namespace KuchCraft {

    Entity::Entity(entt::entity handle, World* world)
        : m_EntityHandle(handle), m_World(world)
    {
    }

}