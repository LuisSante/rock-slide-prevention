#pragma once

#include <engine/common.hpp>
#include <engine/native.hpp>

class Entity2D
{
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
