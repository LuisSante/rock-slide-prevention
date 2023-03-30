#include <entities/rockEntity.hpp>
#include <components/sceneComponent.hpp>
#include <components/spriteComponent.hpp>

#include <managers/componentManager.hpp>

#include <cstdlib>
#include <iostream>

void RockEntity::Init(float r1, float r2, std::ostream& out)
{
    Out = &out;
}

void RockEntity::Destroy()
{
}

void RockEntity::Print()
{
    (*Out) << "-----------------" << std::endl;
    (*Out) << "I'm rock number: " << ID << std::endl;
    (*Out) << "My rotation is: " << RootComponent->Transform.Rotation << " degrees" << std::endl;
    (*Out) << "-----------------" << std::endl;
}

void RockEntity::Init_Internal(uint32 _ID)
{
    ID = _ID;

    SpriteComponent = ComponentManager::CreateComponent<USpriteComponent>(this);
    Components.push_back(SpriteComponent);

    float rot = 360.0f * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    RootComponent->Transform.Rotation = rot;
}
