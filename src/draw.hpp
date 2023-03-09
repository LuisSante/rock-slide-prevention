#ifndef DRAW_HPP
#define DRAW_HPP

#include <math.h>

const float pi = 3.14;

class Draw
{
private:
    float Xd;
    float Yd;
    float a;
    float b;
    double inclinacion = 0;
    int salto_angulos_draw = 30;
    int n_triangulos = 360 / salto_angulos_draw;
    int index_vertices = 3;
    int index_indices = 0;
    int index_indices_value = 1;

public:
    Draw();
    Draw(float Xd, float Yd, float a, float b);
    float radianes(float angulo);
    void vertices_elipse(float vertices[]);
    void indices_elipse(unsigned int indices[]);

    friend class Superposition;
    friend class Intersection;
};

Draw::Draw()
{
    Xd = 0;
    Yd = 0;
    a = 0;
    b = 0;
};

Draw::Draw(float Xd, float Yd, float a, float b)
{
    this->Xd = Xd;
    this->Yd = Yd;
    this->a = a;
    this->b = b;
}

float Draw::radianes(float angulo)
{
    return (angulo * pi) / 180;
}

void Draw::vertices_elipse(float vertices[])
{
    vertices[0] = Xd;
    vertices[1] = Yd;
    vertices[2] = 0.0f;

    // llenar array de vertices (x,y,z=0)
    for (int i = 0; i < n_triangulos; i++)
    {
        inclinacion = inclinacion + salto_angulos_draw;
        vertices[index_vertices] = Xd + float(a * cos(radianes(inclinacion))); // coordenada X
        index_vertices++;
        vertices[index_vertices] = Yd + float(b * sin(radianes(inclinacion))); // coordenada Y
        index_vertices++;
        vertices[index_vertices] = 0.0f; // coordenada Z
        index_vertices++;
    }
}

void Draw::indices_elipse(unsigned int indices[])
{
    // llenar el array de indices
    for (int j = 1; j < n_triangulos; j++)
    {
        indices[index_indices] = 0; // todos los triangulos parten del centro
        index_indices++;
        indices[index_indices] = index_indices_value; // 2do vertices
        index_indices++;
        index_indices_value++;
        indices[index_indices] = index_indices_value; // 3er vertice
        index_indices++;
    }
    indices[index_indices] = index_indices_value;
    index_indices++;
    indices[index_indices] = 0;
    index_indices++;
    indices[index_indices] = 1;
}

#endif