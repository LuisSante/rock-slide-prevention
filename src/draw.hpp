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
    float h = 0.001f;
    float w = 0.2f;
    float t0 = 0.0f;
    float tf = 20.0f;
    float masa = 120.0f;
    float fuerza = 0.0f;
    const float g = 9.81f;
    float MG = 0.0f;

    float I = 0.0f;

public:
    int n;

    Draw();
    Draw(float Xd, float Yd, float a, float b, float vx0, float vy0, float theta0, int number_of_sections);
    float degrees_to_radians(float angulo);
    void vertices_elipse(float vertices[]);
    void indices_elipse(unsigned int indices[]);
    /*void movimiento_proyectil(float t, float Xd, float Yd, float &vx, float &vy, float &theta);
    void move(float delta_pos_x[], float delta_pos_y[], float theta_array[]);*/

    //sumatoria de fuerzas
    float Fx();
    float Fy();
    void runge_kutta(vector<float>& posiciones_x,
                     vector<float>& posiciones_y,
                     vector<float>& velocidades_x,
                     vector<float>& velocidades_y,
                     vector<float>& angulos);

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

    vx0 = 0.0f;
    vy0 = 0.0f;
    theta0 = 0.0f;
    number_of_sections = 0;

    I = 0.0f;
    n = 0;
};

Draw::Draw(float Xd, float Yd, float a, float b, float vx0, float vy0, float theta0, int number_of_sections)
{
    this->Xd = Xd;
    this->Yd = Yd;
    this->a = a;
    this->b = b;
    this->vx0 = vx0;
    this->vy0 = vy0;
    this->theta0 = theta0;
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

// Funciones para calcular las fuerzas en x e y
float Draw::Fx()
{
    return 0.0; // Agregue su función de fuerza en x aquí
}

float Draw::Fy()
{
    return -masa * g; // Agregue su función de fuerza en y aquí
}

// Función para implementar el método de Runge-Kutta de cuarto orden
void Draw::runge_kutta(vector<float>& posiciones_x, vector<float>& posiciones_y,
                       vector<float>& velocidades_x, vector<float>& velocidades_y,
                       vector<float>& angulos)
{
    float x = Xd, y = Yd, vx = vx0, vy = vy0, theta = theta0;
    float k1x, k2x, k3x, k4x, k1y, k2y, k3y, k4y, k1vx, k2vx, k3vx, k4vx, k1vy, k2vy, k3vy, k4vy;
    float k1omega, k2omega, k3omega, k4omega, k1theta, k2theta, k3theta, k4theta;
    float derivada_w = MG / I;

    for (int i = 0; t0 + i * h <= tf; ++i)
    {
        float t = t0 + i * h;

        posiciones_x.push_back(x);
        posiciones_y.push_back(y);
        velocidades_x.push_back(vx);
        velocidades_y.push_back(vy);
        angulos.push_back(theta);

        k1vx = Fx();
        k1vy = Fy() / masa;
        k1x = vx;
        k1y = vy;
        k1omega = derivada_w;
        k1theta = w + derivada_w * t;

        k2vx = Fx();
        k2vy = Fy() / masa;
        k2x = vx + k1vx * h / 2.0f;
        k2y = vy + k1vy * h / 2.0f;
        k2omega = derivada_w;
        k2theta = w + derivada_w * (t + 0.5f * h);

        k3vx = Fx();
        k3vy = Fy() / masa;
        k3x = vx + k2vx * h / 2.0f;
        k3y = vy + k2vy * h / 2.0f;
        k3omega = derivada_w;
        k3theta = w + derivada_w * (t + 0.5f * h);

        k4vx = Fx();
        k4vy = Fy() / masa;
        k4x = vx + k3vx * h;
        k4y = vy + k3vy * h;
        k4omega = derivada_w;
        k4theta = w + derivada_w * (t + h);

        x += (h / 6.0f) * (k1x + 2.0f * k2x + 2.0f * k3x + k4x);
        y += (h / 6.0f) * (k1y + 2.0f * k2y + 2.0f * k3y + k4y);
        vx += (h / 6.0f) * (k1vx + 2.0f * k2vx + 2.0f * k3vx + k4vx);
        vy += (h / 6.0f) * (k1vy + 2.0f * k2vy + 2.0f * k3vy + k4vy);
        theta += (h / 6.0f) * (k1theta + 2.0f * k2theta + 2.0f * k3theta + k4theta);
    }
}

#endif
