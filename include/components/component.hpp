#pragma once

#include <engine/common.hpp>

// Componente padre
struct BaseComponent
{
public:
    struct Entity* OwnerEntity;
};

template <typename T>
struct Component : public BaseComponent
{
public:
    static const uint32 ID;
    static const uint32 SIZE;
};
