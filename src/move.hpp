#ifndef MOVE_HPP
#define MOVE_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <utility>

#include "draw.hpp"

using namespace std;

class Move
{
private:
    Draw draw;

    float mass = 120.0f;
    const float g = 9.81f;
    float I;

public:
    Move();
    Move(Draw &draw);

    float Fx(float Fni, float Fti);
    float Fy(float Fnj, float Ftj);
    void movement(float &velocity_x, float &velocity_y, float &position_x, float &position_y, float &theta, float &w, float &MG, float h, float t, int i, pair<float, float> &Fn, pair<float, float> &Ft);

    friend class Superposition;
    friend class Speed_F_Normal;
};

Move::Move() : draw()
{
    I = 0;
};

Move::Move(Draw &draw) : draw(draw)
{
    I = mass * ((draw.a * draw.b) * (draw.a * draw.a + draw.b * draw.b)) / 4.0f;
}

float Move::Fx(float Fni, float Fti)
{
    float fx = (Fni + Fti + 0) / mass;
    return fx;
}

float Move::Fy(float Fnj, float Ftj)
{
    float fy = (Fnj + Ftj + (-mass * g)) / mass;
    return fy;
}

void Move::movement(float &velocity_x, float &velocity_y, float &position_x, float &position_y, float &theta, float &w, float &MG, float h, float t, int i, pair<float, float> &Fn, pair<float, float> &Ft)
{
    float k1x, k2x, k3x, k4x, k1y, k2y, k3y, k4y, k1vx, k2vx, k3vx, k4vx, k1vy, k2vy, k3vy, k4vy;
    float k1omega, k2omega, k3omega, k4omega, k1theta, k2theta, k3theta, k4theta;
    float derivative_w = MG / I;

    k1vx = Fx(Fn.first, Ft.first);
    k1vy = Fy(Fn.second, Ft.second);
    k1x = velocity_x;
    k1y = velocity_y;
    k1omega = derivative_w;
    k1theta = w + derivative_w * t;

    k2vx = Fx(Fn.first, Ft.first);
    k2vy = Fy(Fn.second, Ft.second);
    k2x = velocity_x + k1vx * h / 2.0f;
    k2y = velocity_y + k1vy * h / 2.0f;
    k2omega = derivative_w;
    k2theta = w + derivative_w * (t + 0.5f * h);

    k3vx = Fx(Fn.first, Ft.first);
    k3vy = Fy(Fn.second, Ft.second);
    k3x = velocity_x + k2vx * h / 2.0f;
    k3y = velocity_y + k2vy * h / 2.0f;
    k3omega = derivative_w;
    k3theta = w + derivative_w * (t + 0.5f * h);

    k4vx = Fx(Fn.first, Ft.first);
    k4vy = Fy(Fn.second, Ft.second);
    k4x = velocity_x + k3vx * h;
    k4y = velocity_y + k3vy * h;
    k4omega = derivative_w;
    k4theta = w + derivative_w * (t + h);

    position_x += (h / 6.0f) * (k1x + 2.0f * k2x + 2.0f * k3x + k4x);
    position_y += (h / 6.0f) * (k1y + 2.0f * k2y + 2.0f * k3y + k4y);
    velocity_x += (h / 6.0f) * (k1vx + 2.0f * k2vx + 2.0f * k3vx + k4vx);
    velocity_y += (h / 6.0f) * (k1vy + 2.0f * k2vy + 2.0f * k3vy + k4vy);
    theta += (h / 6.0f) * (k1theta + 2.0f * k2theta + 2.0f * k3theta + k4theta);

    //cout << velocity_x << " " << velocity_y << endl;
}

#endif