#pragma once

#include <entities/entity.hpp>

#include <iostream>

template <typename E>
class EntityManager;

class RockEntity : public Entity2D
{
    friend class EntityManager<RockEntity>;
public:
    virtual void Init(float r1, float r2, std::ostream& out = std::cout);
    virtual void Destroy() override;
    virtual void Print() override;

protected:
    virtual void Init_Internal(uint32 _ID) override;

public:
    struct USpriteComponent* SpriteComponent;

private:
    std::ostream* Out;
};
