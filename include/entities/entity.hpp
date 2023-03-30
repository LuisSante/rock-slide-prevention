#pragma once

#include <engine/common.hpp>
#include <engine/native.hpp>

template <typename E>
class EntityManager;

class Entity2D
{
    friend class EntityManager<Entity2D>;
public:
    virtual void Destroy() = 0;
    virtual void Print() = 0;

protected:
    virtual void Init_Internal(uint32 _ID) = 0;

public:
    uint32 ID;

    struct USceneComponent2D* RootComponent;

protected:
    TArray<struct BaseComponent*> Components;
};

class Entity
{
    friend class EntityManager<Entity>;
public:
    virtual void Destroy() = 0;
    virtual void Print() = 0;

protected:
    virtual void Init_Internal(uint32 _ID) = 0;

public:
    uint32 ID;

    struct USceneComponent* RootComponent;

protected:
    TArray<struct BaseComponent*> Components;
};
