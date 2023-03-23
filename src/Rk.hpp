#ifndef RK_HPP
#define RK_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <utility>

#include "draw.hpp"

using namespace std;

ofstream Frame_Rk("src/reportes/frame_RK.txt");

class RungeKutta
{
private:
    // Atributos de la elipse
    Draw draw;
    // RK
    float masa = 120.0f;
    float fuerza = 0.0f;
    const float g = 9.81f;
    float I;

public:
    RungeKutta();
    RungeKutta(Draw &draw);

    // sumatoria de fuerzas
    float Fx(float Fni, float Fti);
    float Fy(float Fnj, float Ftj);
    void runge_kutta(float &velocity_x, float &velocity_y, float &position_x, float &position_y, float &theta, float &w, float &MG, float h, float t, int i, pair<float, float> Fn, pair<float, float> Ft);

    friend class Superposition;
    friend class PuntoContacto;
    friend class Speed_F_Normal;
};

RungeKutta::RungeKutta() : draw()
{
    I = 0;
};

RungeKutta::RungeKutta(Draw &draw) : draw(draw)
{
    I = masa * ((draw.a * draw.b) * (draw.a * draw.a + draw.b * draw.b)) / 4.0f;
    cout << "Inercia "<< I << endl;
}

// Funciones para calcular las fuerzas en x e y
float RungeKutta::Fx(float Fni, float Fti)
{
    float fx = (Fni + Fti + 0) / masa;
    //cout << "CLASE RK "
    //    << "FN_i : " << Fni << " Ft_i : " << Fti << " Fx : " << fx <<endl;
    return fx;
}

float RungeKutta::Fy(float Fnj, float Ftj)
{
    float fy = (Fnj + Ftj + (-masa * g)) / masa;
    //cout << "CLASE RK "
    //     << "FN_j : " << Fnj << " Ft_j : " << Ftj << " Fy : " << fy <<endl;
    return fy;
}

// Función para implementar el método de Runge-Kutta de cuarto orden
void RungeKutta::runge_kutta(float &velocity_x, float &velocity_y, float &position_x, float &position_y, float &theta, float &w, float &MG, float h, float t, int i, pair<float, float> Fn, pair<float, float> Ft)
{
    // float x = Xd, y = Yd, vx = vx0, vy = vy0, theta = theta0;
    float k1x, k2x, k3x, k4x, k1y, k2y, k3y, k4y, k1vx, k2vx, k3vx, k4vx, k1vy, k2vy, k3vy, k4vy;
    float k1omega, k2omega, k3omega, k4omega, k1theta, k2theta, k3theta, k4theta;
    float derivada_w = MG / I;

    // float t = t0 + i * h;
    Frame_Rk << t << " " << position_x << " " << position_y << " " << velocity_x << " " << velocity_y << " " << theta << endl;

    k1vx = Fx(Fn.first, Ft.first);
    k1vy = Fy(Fn.second, Ft.second);
    k1x = velocity_x;
    k1y = velocity_y;
    k1omega = derivada_w;
    k1theta = w + derivada_w * t;

    k2vx = Fx(Fn.first, Ft.first);
    k2vy = Fy(Fn.second, Ft.second);
    k2x = velocity_x + k1vx * h / 2.0f;
    k2y = velocity_y + k1vy * h / 2.0f;
    k2omega = derivada_w;
    k2theta = w + derivada_w * (t + 0.5f * h);

    k3vx = Fx(Fn.first, Ft.first);
    k3vy = Fy(Fn.second, Ft.second);
    k3x = velocity_x + k2vx * h / 2.0f;
    k3y = velocity_y + k2vy * h / 2.0f;
    k3omega = derivada_w;
    k3theta = w + derivada_w * (t + 0.5f * h);

    k4vx = Fx(Fn.first, Ft.first);
    k4vy = Fy(Fn.second, Ft.second);
    k4x = velocity_x + k3vx * h;
    k4y = velocity_y + k3vy * h;
    k4omega = derivada_w;
    k4theta = w + derivada_w * (t + h);

    /*out << "CLASE RK "
         << "Fx : " << Fx() << " Fy : " << Fy() << endl;*/

    position_x += (h / 6.0f) * (k1x + 2.0f * k2x + 2.0f * k3x + k4x);
    position_y += (h / 6.0f) * (k1y + 2.0f * k2y + 2.0f * k3y + k4y);
    velocity_x += (h / 6.0f) * (k1vx + 2.0f * k2vx + 2.0f * k3vx + k4vx);
    velocity_y += (h / 6.0f) * (k1vy + 2.0f * k2vy + 2.0f * k3vy + k4vy);
    theta += (h / 6.0f) * (k1theta + 2.0f * k2theta + 2.0f * k3theta + k4theta);
}

#endif