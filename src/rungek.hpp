#ifndef RUNGE_K_HPP
#define RUNGE_K_HPP

#include <iostream>
#include <cmath>

using namespace std;

const int size_ = 21;

class RungeKutta
{

private:
    float w = 0.2;
    float t = 0.0f;
    float Xd;
    float Yd;
    float vx;
    float vy;
    float theta;
    const float g = 9.81;
    float a;
    float b;
    float dt;
    int n;
    float x_array[size_];
    float y_array[size_];
    float vx_array[size_];
    float vy_array[size_];

public:
    RungeKutta();
    RungeKutta(float Xd, float Yd, float vx, float vy, float theta, float a, float b, float dt, int n);
    void movimiento_proyectil(float t, float Xd, float Yd, float &vx, float &vy, float &theta);
    void move(float delta_pos_x[], float delta_pos_y[], float theta_array[]);

    friend class Speed;
};

RungeKutta::RungeKutta()
{
    Xd = 0;
    Yd = 0;
    vx = 0;
    vy = 0;
    theta = 0;
    a = 0;
    b = 0;
    dt = 0;
    n = 0;
}

RungeKutta::RungeKutta(float Xd, float Yd, float vx, float vy, float theta, float a, float b, float dt, int n)
{
    this->Xd = Xd;
    this->Yd = Yd;
    this->vx = vx;
    this->vy = vy;
    this->theta = theta;
    this->a = a;
    this->b = b;
    this->dt = dt;
    this->n = n;
    x_array[0] = Xd;
    y_array[0] = Yd;
    vx_array[0] = vx;
    vy_array[0] = vy;
}

// Definir la función que describe el movimiento del proyectil
void RungeKutta::movimiento_proyectil(float t, float Xd, float Yd, float &vx, float &vy, float &theta)
{
    // Calcular las aceleraciones en los ejes x e y
    float ax = 0.0;
    float ay = -g;

    // Calcular la fuerza de resistencia del aire
    float r_aire = 0.000; // coeficiente de resistencia del aire
    float Fx = -r_aire * vx * std::sqrt(vx * vx + vy * vy);
    float Fy = -r_aire * vy * std::sqrt(vx * vx + vy * vy);

    // Calcular el momento resultante en el plano xy
    float I = (0.25 * 3.14) * (a * b) * (a * a + b * b); // momento de inercia del proyectil (en kg*m^2)
    float M = 0;                                         // momento resultante

    // Actualizar la velocidad en el eje x
    float k1_vx = ax + Fx / I;
    float k2_vx = ax + Fx / I;
    float k3_vx = ax + Fx / I;
    float k4_vx = ax + Fx / I;
    vx += (k1_vx + 2.0 * k2_vx + 2.0 * k3_vx + k4_vx) / 6.0;

    // Actualizar la velocidad en el eje y
    float k1_vy = ay + Fy / I;
    float k2_vy = ay + Fy / I;
    float k3_vy = ay + Fy / I;
    float k4_vy = ay + Fy / I;
    vy += (k1_vy + 2.0 * k2_vy + 2.0 * k3_vy + k4_vy) / 6.0;

    // Actualizar la posición en el eje x
    float k1_x = vx;
    float k2_x = vx + k1_vx / 2.0;
    float k3_x = vx + k2_vx / 2.0;
    float k4_x = vx + k3_vx;
    Xd += (k1_x + 2.0 * k2_x + 2.0 * k3_x + k4_x) / 6.0;

    // Actualizar la posición en el eje y
    float k1_y = vy;
    float k2_y = vy + k1_vy / 2.0;
    float k3_y = vy + k2_vy / 2.0;
    float k4_y = vy + k3_vy;
    Yd += (k1_y + 2.0 * k2_y + 2.0 * k3_y + k4_y) / 6.0;

    // Actualizar el ángulo de giro del proyectil
    // suma W ya que es ek resultado del primer RK
    float k1_theta = w;
    float k2_theta = w + M / I;
    float k3_theta = w + M / I;
    float k4_theta = w + M / I;
    theta += (k1_theta + 2.0 * k2_theta + 2.0 * k3_theta + k4_theta) / 6.0;
}

void RungeKutta::move(float delta_pos_x[], float delta_pos_y[], float theta_array[])
{
    theta_array[0] = theta;
    for (int i = 1; i < n; i++)
    {
        // Calcular los valores de k1, k2, k3 y k4 para la actualización de las variables
        float k1_x, k2_x, k3_x, k4_x, k1_y, k2_y, k3_y, k4_y;
        float k1_vx, k2_vx, k3_vx, k4_vx, k1_vy, k2_vy, k3_vy, k4_vy;
        float k1_theta, k2_theta, k3_theta, k4_theta;

        k1_x = vx;
        k1_vx = 0.0;
        k1_y = vy;
        k1_vy = 0.0;
        k1_theta = 0.0;
        movimiento_proyectil(t, Xd, Yd, k1_vx, k1_vy, k1_theta);
        k2_x = vx + k1_vx * dt / 2.0;
        k2_vx = 0.0;
        k2_y = vy + k1_vy * dt / 2.0;
        k2_vy = 0.0;
        k2_theta = 0.0;
        movimiento_proyectil(t + dt / 2.0, Xd + k1_x * dt / 2.0, Yd + k1_y * dt / 2.0, k2_vx, k2_vy, k2_theta);

        k3_x = vx + k2_vx * dt / 2.0;
        k3_vx = 0.0;
        k3_y = vy + k2_vy * dt / 2.0;
        k3_vy = 0.0;
        k3_theta = 0.0;
        movimiento_proyectil(t + dt / 2.0, Xd + k2_x * dt / 2.0, Yd + k2_y * dt / 2.0, k3_vx, k3_vy, k3_theta);
        k4_x = vx + k3_vx * dt;
        k4_vx = 0.0;
        k4_y = vy + k3_vy * dt;
        k4_vy = 0.0;
        k4_theta = 0.0;
        movimiento_proyectil(t + dt, Xd + k3_x * dt, Yd + k3_y * dt, k4_vx, k4_vy, k4_theta);

        // Actualizar las variables con el método Runge-Kutta de cuarto orden
        Xd += (k1_x + 2.0 * k2_x + 2.0 * k3_x + k4_x) / 6.0;
        vx += (k1_vx + 2.0 * k2_vx + 2.0 * k3_vx + k4_vx) / 6.0;
        Yd += (k1_y + 2.0 * k2_y + 2.0 * k3_y + k4_y) / 6.0;
        vy += (k1_vy + 2.0 * k2_vy + 2.0 * k3_vy + k4_vy) / 6.0;
        theta += (k1_theta + 2.0 * k2_theta + 2.0 * k3_theta + k4_theta) / 6.0;

        // Almacenar los valores de las posiciones y velocidades en los arrays correspondientes
        x_array[i] = Xd;
        vx_array[i] = vx;
        y_array[i] = Yd;
        vy_array[i] = vy;
        theta_array[i] = theta;

        // Actualizar el tiempo
        t += dt;
    }

    /*for(int i = 0; i < n; i++){
        cout<<theta_array[i]<<endl;
    }
    cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl;*/

    delta_pos_x[0] = x_array[0];
    delta_pos_y[0] = y_array[0];;
    for (int i = 1; i < n; i++)
    {
        delta_pos_x[i] = (x_array[i] - x_array[i - 1]) / 3500;
        delta_pos_y[i] = (y_array[i] - y_array[i - 1]) / 3500;
    }
}

#endif