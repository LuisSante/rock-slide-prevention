#pragma once

#include <components/component.hpp>

struct USpriteComponent : public Component<USpriteComponent>
{
public:
    FLinearColor Color;
    FResource* VertexArrayResource;
};
