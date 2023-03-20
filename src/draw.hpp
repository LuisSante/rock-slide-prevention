#ifndef DRAW_HPP
#define DRAW_HPP

#include <vector>
#include <iostream>

#include <cmath>
#ifdef M_PI
    #undef M_PI
#endif
#define M_PI 3.14159265358979323846f

using std::vector;

class Draw
{
private:
    float a = 0.0f;
    float b = 0.0f;
    float Xd = 0.0f;
    float Yd = 0.0f;
    int number_of_sections = 0;

    // RK
    float vx0 = 0.0f;
    float vy0 = 0.0f;
    float theta0 = 0.0f;
    float h = 1.0f;
    float w = 0.2f;
    float t0 = 0.0f;
    float tf = 20.0f;
    float masa = 120.0f;
    float fuerza = 0.0f;
    const float g = 9.81f;
    float MG = 0.0f;
    float I = 0.0f;

    //fuerzas externas

public:
    int n;

    Draw();
    Draw(float Xd, float Yd, float a, float b, int number_of_sections);
    float degrees_to_radians(float angulo);
    void vertices_elipse(float vertices[]);
    void indices_elipse(unsigned int indices[]);
    /*void movimiento_proyectil(float t, float Xd, float Yd, float &vx, float &vy, float &theta);
    void move(float delta_pos_x[], float delta_pos_y[], float theta_array[]);*/

    friend class RungeKutta;
    friend class Superposition;
    friend class PuntoContacto;
    friend class Speed_F_Normal;
};

Draw::Draw()
{
    Xd = 0.0f;
    Yd = 0.0f;
    a = 0.0f;
    b = 0.0f;

    number_of_sections = 0;

    I = 0.0f;
    n = 0;
};

Draw::Draw(float Xd, float Yd, float a, float b, int number_of_sections)
{
    this->Xd = Xd;
    this->Yd = Yd;
    this->a = a;
    this->b = b;
    /*this->vx0 = vx0;
    this->vy0 = vy0;
    this->theta0 = theta0;*/
    this->number_of_sections = number_of_sections;

    I = masa * (a * b) * (a * a + b * b) / 4.0f;
    n = (tf - t0) / h;
}

float Draw::degrees_to_radians(float angulo)
{
    return (angulo * M_PI) / 180.0f;
}

void Draw::vertices_elipse(float vertex_data[])
{
    float angle_step = 360.0f / static_cast<float>(number_of_sections);

    vertex_data[0] = Xd;
    vertex_data[1] = Yd;
    vertex_data[2] = 0.0f;

    vertex_data[3] = 0.0f;
    vertex_data[4] = 0.0f;
    vertex_data[5] = 1.0f;

    // llenar array de vertices (x,y,z=0)
    float angle = 0.0f;
    for (int i = 1; i < number_of_sections + 1; ++i)
    {
        vertex_data[i * 6 + 0] = Xd + (a * cosf(degrees_to_radians(angle))); // coordenada X
        vertex_data[i * 6 + 1] = Yd + (b * sinf(degrees_to_radians(angle))); // coordenada Y
        vertex_data[i * 6 + 2] = 0.0f; // coordenada Z

        vertex_data[i * 6 + 3] = 0.0f;
        vertex_data[i * 6 + 4] = 0.0f;
        vertex_data[i * 6 + 5] = 1.0f;

        angle += angle_step;
    }
}

void Draw::indices_elipse(unsigned int indices[])
{
    // llenar el array de indices
    int current_index_base = 1;
    for (int j = 0; j < number_of_sections; ++j)
    {
        indices[j * 3 + 0] = 0; // todos los triangulos parten del centro
        indices[j * 3 + 1] = current_index_base + 0; // 2do vertices
        indices[j * 3 + 2] = current_index_base + 1; // 3er vertice
        ++current_index_base;
    }
    indices[(number_of_sections - 1) * 3 + 2] = 1;
}

#endif
