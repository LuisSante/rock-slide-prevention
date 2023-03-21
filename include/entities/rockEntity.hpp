#pragma once

#include <entities/entity.hpp>

class RockEntity : public Entity
{
public:
    RockEntity();

    virtual void Init(float r1, float r2);
    virtual void Destroy() override;
    virtual void Print() override;

protected:
    virtual void Init_Internal(uint32 _ID) override;

public:
    struct USpriteComponent* SpriteComponent;
};
