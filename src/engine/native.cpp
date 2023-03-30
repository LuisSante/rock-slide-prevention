#include <engine/native.hpp>

const FVector2D FVector2D::ZeroVector{ 0.0f, 0.0f };
const FVector2D FVector2D::OneVector{ 1.0f, 1.0f };

const FVector FVector::ZeroVector{ 0.0f, 0.0f, 0.0f };
const FVector FVector::OneVector{ 1.0f, 1.0f, 1.0f };
const FVector FVector::UpVector{ 0.0f, 0.0f, 1.0f };
const FVector FVector::RightVector{ 0.0f, 1.0f, 0.0f };
const FVector FVector::ForwardVector{ 1.0f, 0.0f, 0.0f };

const FLinearColor FLinearColor::Red{ 1.0f, 0.0f, 0.0f };
const FLinearColor FLinearColor::Green{ 0.0f, 1.0f, 0.0f };
const FLinearColor FLinearColor::Blue{ 0.0f, 0.0f, 1.0f };

const FQuat FQuat::Identity{ 1.0f, 0.0f, 0.0f, 0.0f };

uint32 FResource::Count{ 0 };
