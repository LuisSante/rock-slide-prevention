#ifndef DRAW_HPP
#define DRAW_HPP

#include <math.h>

const float pi = 3.14;

float radianes(float angulo)
{
    return (angulo * pi) / 180;
}

void vertices_elipse(float origen_x, float origen_y, float escala_x, float escala_y, double inclinacion, const int salto_angulos_draw, int n_triangulos, int index_vertices, float vertices[])
{
    vertices[0] = origen_x;
    vertices[1] = origen_y;
    vertices[2] = 0.0f;

    // llenar array de vertices (x,y,z=0)
    for (int i = 0; i < n_triangulos; i++)
    {
        inclinacion = inclinacion + salto_angulos_draw;
        vertices[index_vertices] = origen_x + float(escala_x * cos(radianes(inclinacion))); // coordenada X
        index_vertices++;
        vertices[index_vertices] = origen_y + float(escala_y * sin(radianes(inclinacion))); // coordenada Y
        index_vertices++;
        vertices[index_vertices] = 0.0f; // coordenada Z
        index_vertices++;
    }
}

void indices_elipse(int n_triangulos, int index_indices, int index_indices_value, unsigned int indices[])
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