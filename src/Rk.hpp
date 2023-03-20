#ifndef RK_HPP
#define RK_HPP

#include <iostream>
#include <vector>

#include "draw.hpp"
#include "speed_f_normal.hpp"

using namespace std;

class RungeKutta
{
private:
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

    // Atributos de la elipse
    Draw draw;

    // Atributos para cambio de rotacion
    Speed_F_Normal speed;

public:
    int n;

    RungeKutta();
    RungeKutta(float vx0, float vy0, float theta0, Draw draw, Speed_F_Normal speed);

    // sumatoria de fuerzas
    float Fx();
    float Fy();
    void runge_kutta(vector<float> &posiciones_x,
                     vector<float> &posiciones_y,
                     vector<float> &velocidades_x,
                     vector<float> &velocidades_y,
                     vector<float> &angulos);

    friend class Superposition;
    friend class PuntoContacto;
    friend class Speed_F_Normal;
};

RungeKutta::RungeKutta() : draw() , speed()
{
    vx0 = 0.0f;
    vy0 = 0.0f;
    theta0 = 0.0f;

    I = 0.0f;
    n = 0;
};

RungeKutta::RungeKutta(float vx0, float vy0, float theta0, Draw draw, Speed_F_Normal speed) : draw(draw) , speed(speed)
{
    this->vx0 = vx0;
    this->vy0 = vy0;
    this->theta0 = theta0;

    I = masa * (draw.a * draw.b) * (draw.a * draw.a + draw.b * draw.b) / 4.0f;
    n = (tf - t0) / h;
}

// Funciones para calcular las fuerzas en x e y
float RungeKutta::Fx()
{
    float fx = (speed.FN_i + speed.Ft_i + 0)/masa;
    return fx; 
}

float RungeKutta::Fy()
{
    float fy = (speed.FN_j + speed.Ft_j + (-masa * g))/masa;
    return fy; 
}

// Función para implementar el método de Runge-Kutta de cuarto orden
void RungeKutta::runge_kutta(vector<float> &posiciones_x, vector<float> &posiciones_y,
                             vector<float> &velocidades_x, vector<float> &velocidades_y,
                             vector<float> &angulos)
{
    float x = draw.Xd, y = draw.Yd, vx = vx0, vy = vy0, theta = theta0;
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
        k1vy = Fy();
        k1x = vx;
        k1y = vy;
        k1omega = derivada_w;
        k1theta = w + derivada_w * t;

        k2vx = Fx();
        k2vy = Fy();
        k2x = vx + k1vx * h / 2.0f;
        k2y = vy + k1vy * h / 2.0f;
        k2omega = derivada_w;
        k2theta = w + derivada_w * (t + 0.5f * h);

        k3vx = Fx();
        k3vy = Fy();
        k3x = vx + k2vx * h / 2.0f;
        k3y = vy + k2vy * h / 2.0f;
        k3omega = derivada_w;
        k3theta = w + derivada_w * (t + 0.5f * h);

        k4vx = Fx();
        k4vy = Fy();
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