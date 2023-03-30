#pragma once

#include <engine/common.hpp>
#include <engine/native.hpp>

#include <components/component.hpp>

#include <iostream>
#include <type_traits>

class ComponentManagerGarbageCollector
{
    friend class ComponentManager;
public:
    ~ComponentManagerGarbageCollector();

private:
    inline explicit ComponentManagerGarbageCollector() {}
};

class ComponentManager
{
    friend class ComponentManagerGarbageCollector;
public:
    static uint32 RegisterComponent();

    template <typename C>
    static C* CreateComponent(Entity2D* entity);

    template <typename C>
    static bool DeleteComponent(Entity2D* entity);

    template <typename C>
    static TList<BaseComponent*>& GetComponentBuffer();

private:
    explicit ComponentManager() {};

private:
    static uint32 ComponentID;
    static uint32 GlobalComponentCount;
    static TArray<TList<BaseComponent*>> ComponentBuffers;

    static ComponentManagerGarbageCollector GarbageCollector;
};

#include "componentManager.inl"
