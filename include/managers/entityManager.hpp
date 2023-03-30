#pragma once

#include <engine/common.hpp>
#include <engine/native.hpp>

#include <entities/entity.hpp>

#include <components/sceneComponent.hpp>
#include <managers/componentManager.hpp>

#include <type_traits>

template <typename E>
class EntityManager
{
public:
    inline explicit EntityManager() : CurrentID { 0 }, Entities {} {}
    ~EntityManager();

    E* CreateEntity();
    bool RemoveEntity(E* entity);
    uint32 Clear();

    void PrintEntities();

private:
    uint32 CurrentID;
    TList<Entity2D*> Entities;

    static uint32 GlobalEntityCount;
};

#include "entityManager.inl"
