#ifndef DRAW_HPP
#define DRAW_HPP

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

const float pi = 3.14;

class Draw
{
private:
    float Xd;
    float Yd;
    float a;
    float b;
    double inclinacion = 0.0f;
    int salto_angulos_draw = 20;
    int n_triangulos = 360 / salto_angulos_draw;
    int index_vertices = 3;
    int index_indices = 0;
    int index_indices_value = 1;

    // RK
    float vx0;
    float vy0;
    float theta0;
    float h = 0.000001;
    float w = 0.5f;
    float t0 = 0.0f;
    float tf = 20.0f;
    float masa = 120.0f;
    float fuerza = 0.0f;
    const float g = 9.81f;
    float I = masa * (a * b )* (a * a + b * b) / 4.0;

public:
    int n = (tf - t0) / h;

    Draw();
    Draw(float Xd, float Yd, float a, float b, float vx, float vy, float theta);
    float radianes(float angulo);
    void vertices_elipse(float vertices[]);
    void indices_elipse(unsigned int indices[]);
    /*void movimiento_proyectil(float t, float Xd, float Yd, float &vx, float &vy, float &theta);
    void move(float delta_pos_x[], float delta_pos_y[], float theta_array[]);*/

    double Fx();
    double Fy();
    void runge_kutta(vector<float>& posiciones_x , vector<float>& posiciones_y, vector<float>& velocidades_x , vector<float>& velocidades_y , vector<float>& angulos);

    friend class Superposition;
    friend class PuntoContacto;
    friend class Speed_F_Normal;
};

Draw::Draw()
{
    Xd = 0;
    Yd = 0;
    a = 0;
    b = 0;

    vx0 = 0;
    vy0 = 0;
    theta0 = 0;
};

Draw::Draw(float Xd, float Yd, float a, float b, float vx0, float vy0, float theta0)
{
    this->Xd = Xd;
    this->Yd = Yd;
    this->a = a;
    this->b = b;
    this->vx0 = vx0;
    this->vy0 = vy0;
    this->theta0 = theta0;
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

// Funciones para calcular las fuerzas en x e y
double Draw::Fx()
{
    return 0; // Agregue su función de fuerza en x aquí
}

double Draw::Fy()
{
    return -masa * g; // Agregue su función de fuerza en y aquí
}

// Función para implementar el método de Runge-Kutta de cuarto orden
void Draw::runge_kutta(vector<float>& posiciones_x , vector<float>& posiciones_y, vector<float>& velocidades_x , vector<float>& velocidades_y , vector<float>& angulos)
{
    double x = Xd, y = Yd, vx = vx0, vy = vy0, theta = theta0;
    double k1x, k2x, k3x, k4x, k1y, k2y, k3y, k4y, k1vx, k2vx, k3vx, k4vx, k1vy, k2vy, k3vy, k4vy;
    double k1omega, k2omega, k3omega, k4omega, k1theta, k2theta, k3theta, k4theta;
    double MG = 0;
    double derivada_w = MG / I;

    for (int i = 0; t0 + i * h <= tf; i++)
    {
        double t = t0 + i * h;
        //cout<<"T: "<<t<<endl;
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
        k2x = vx + k1vx * h / 2.0;
        k2y = vy + k1vy * h / 2.0;
        k2omega = derivada_w;
        k2theta = w + derivada_w * (t + 0.5 * h);

        k3vx = Fx();
        k3vy = Fy() / masa;
        k3x = vx + k2vx * h / 2.0;
        k3y = vy + k2vy * h / 2.0;
        k3omega = derivada_w;
        k3theta = w + derivada_w * (t + 0.5 * h);

        k4vx = Fx();
        k4vy = Fy() / masa;
        k4x = vx + k3vx * h;
        k4y = vy + k3vy * h;
        k4omega = derivada_w;
        k4theta = w + derivada_w * (t + h);

        x += (h / 6.0) * (k1x + 2.0 * k2x + 2.0 * k3x + k4x);
        y += (h / 6.0) * (k1y + 2.0 * k2y + 2.0 * k3y + k4y);
        vx += (h / 6.0) * (k1vx + 2.0 * k2vx + 2.0 * k3vx + k4vx);
        vy += (h / 6.0) * (k1vy + 2.0 * k2vy + 2.0 * k3vy + k4vy);
        theta += (h / 6.0) * (k1theta + 2.0 * k2theta + 2.0 * k3theta + k4theta);
    }
}

#endif