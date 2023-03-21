#pragma once

#include <engine/native.hpp>
#include <components/component.hpp>

struct USceneComponent2D : public Component<USceneComponent2D>
{
public:
    FTransform2D Transform;
};

struct USceneComponent : public Component<USceneComponent>
{
public:
    FTransform Transform;
};
