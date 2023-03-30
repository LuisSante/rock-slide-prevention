#pragma once

#include <engine/common.hpp>

#include <list>
#include <vector>

template <typename T>
using TList = std::list<T>;

template <typename T>
using TArray = std::vector<T>;

struct FVector2D
{
public:
    float X;
    float Y;

public:
    static const FVector2D ZeroVector;
    static const FVector2D OneVector;
};

struct FVector
{
public:
    float X;
    float Y;
    float Z;

public:
    static const FVector ZeroVector;
    static const FVector OneVector;
    static const FVector UpVector;
    static const FVector RightVector;
    static const FVector ForwardVector;
};

struct FLinearColor
{
public:
    float R;
    float G;
    float B;

public:
    static const FLinearColor Red;
    static const FLinearColor Green;
    static const FLinearColor Blue;
};

struct FQuat
{
public:
    float W;
    float X;
    float Y;
    float Z;

public:
    static const FQuat Identity;
};

struct FTransform2D
{
public:
    float Rotation;
    FVector2D Scale2D;
    FVector2D Translation;
};

struct FTransform
{
public:
    FQuat Rotation;
    FVector Scale3D;
    FVector Translation;
};

struct FResource
{
public:
    bool bIsGarbageCollectable;

public:
    static uint32 Count; // Cantiadad de referencias a ese recurso
};

struct FVertexArray : public FResource
{
public:
    uint32 VAO_Internal;
    uint32 VBO_Internal;
    uint32 EBO_Internal;
};
