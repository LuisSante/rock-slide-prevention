#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include <utility>
#include <fstream>

#include <cmath>

#include "draw.hpp"

using namespace std;

std::ofstream impact("../src/reports/impact.txt");

class PointContact
{
private:
    Draw draw;
    float vertices_difference[6];
    float vertices_locales_slope[6];
    float matrix_angles[2][2];
    float inverse[2][2];

public:
    bool collision;
    pair<float, float> perpendicular; // Sigma n

public:
    PointContact();
    PointContact(Draw &draw);

public:
    void print_matrix(float matrix[2][2]);

    float degrees_to_radians(float angle) { return (angle * M_PI) / 180.0f; }

    void fill_matrix(float angle);
    void calculate_inverse();

    vector<float> locales(float current_center_mass_X, float current_center_mass_Y, float vertices_slope_[], float inverse[2][2]);
    vector<float> point_collision(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[], float theta);

    void superposition(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[], float theta);
};

PointContact::PointContact()
{
    for (int i = 0; i < 6; i++)
    {
        vertices_difference[i] = 0;
        vertices_locales_slope[i] = 0;
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            matrix_angles[i][j] = 0;
            inverse[i][j] = 0;
        }
    }

    collision = false;
    perpendicular.first = 0.0f;
    perpendicular.second = 0.0f;
}

PointContact::PointContact(Draw &draw) : draw(draw)
{
    for (int i = 0; i < 6; i++)
    {
        vertices_difference[i] = 0;
        vertices_locales_slope[i] = 0;
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            matrix_angles[i][j] = 0;
            inverse[i][j] = 0;
        }
    }

    collision = false;
    perpendicular.first = 0.0f;
    perpendicular.second = 0.0f;
}

void PointContact::print_matrix(float matrix[2][2])
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

void PointContact::fill_matrix(float angle)
{
    float angle_rad = degrees_to_radians(angle);

    matrix_angles[0][0] = cos(angle_rad);
    matrix_angles[0][1] = -sin(angle_rad);
    matrix_angles[1][0] = sin(angle_rad);
    matrix_angles[1][1] = cos(angle_rad);
}

void PointContact::calculate_inverse()
{
    inverse[0][0] = matrix_angles[0][0];
    inverse[0][1] = -matrix_angles[0][1];
    inverse[1][0] = -matrix_angles[1][0];
    inverse[1][1] = matrix_angles[1][1];
}

vector<float> PointContact::locales(float current_center_mass_X, float current_center_mass_Y, float vertices_slope_[], float inverse[2][2])
{
    vector<float> local_middles;

    for (int i = 0; i < 6; i += 3)
    {
        vertices_difference[i] = current_center_mass_X;
        vertices_difference[i + 1] = current_center_mass_Y;
        vertices_difference[i + 2] = 0;
    }

    for (int i = 0; i < 6; i++)
    {
        vertices_locales_slope[i] = vertices_slope_[i] - vertices_difference[i];
    }

    float x1_ellipse_local = vertices_locales_slope[0];
    float y1_ellipse_local = vertices_locales_slope[1];
    float x2_ellipse_local = vertices_locales_slope[3];
    float y2_ellipse_local = vertices_locales_slope[4];

    float x1_ = (inverse[0][0] * x1_ellipse_local) + (inverse[0][1] * y1_ellipse_local);
    float y1_ = (inverse[1][0] * x1_ellipse_local) + (inverse[1][1] * y1_ellipse_local);
    float x2_ = (inverse[0][0] * x2_ellipse_local) + (inverse[0][1] * y2_ellipse_local);
    float y2_ = (inverse[1][0] * x2_ellipse_local) + (inverse[1][1] * y2_ellipse_local);

    local_middles.push_back(x1_);
    local_middles.push_back(y1_);
    local_middles.push_back(x2_);
    local_middles.push_back(y2_);

    return local_middles;
}

vector<float> PointContact::point_collision(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[], float theta)
{
    vector<float> middle;

    fill_matrix(theta);
    calculate_inverse();

    vector<float> local_middles = locales(current_center_mass_X, current_center_mass_Y, vertices_slope, inverse);

    float ml = (draw.a / draw.b) * ((local_middles[3] - local_middles[1]) / (local_middles[2] - local_middles[0]));
    float bl = (local_middles[1] / draw.b) - (ml * (local_middles[0] / draw.a));

    middle.push_back(ml);
    middle.push_back(bl);

    // Quadratic equation
    double e_a = ((ml * ml) + 1);
    double e_b = 2 * ml * bl;
    double e_c = ((bl * bl) - 1);

    double s1 = 0, s2 = 0;
    double discriminant = (e_b * e_b) - 4 * e_a * e_c;

    impact << "discriminant : " << discriminant << endl;

    if (discriminant > 0 || discriminant == 0)
    {
        double discriminant_root = sqrt(discriminant);
        s1 = (-e_b + discriminant_root) / (2 * e_a);
        s2 = (-e_b - discriminant_root) / (2 * e_a);
        collision = true;
    }
    else
    {
        collision = false;
    }

    double p1 = 0, p2 = 0;
    double _x_1 = 0, _y_1 = 0, _x_2 = 0, _y_2 = 0;
    p1 = (ml * s1) + bl;
    p2 = (ml * s2) + bl;

    _x_1 = s1 * draw.a;
    _y_1 = p1 * draw.b;
    _x_2 = s2 * draw.a;
    _y_2 = p2 * draw.b;

    double _x1_ = ((matrix_angles[0][0] * _x_1) + (matrix_angles[0][1] * _y_1)) + current_center_mass_X;
    double _y1_ = ((matrix_angles[1][0] * _x_1) + (matrix_angles[1][1] * _y_1)) + current_center_mass_Y;
    double _x2_ = ((matrix_angles[0][0] * _x_2) + (matrix_angles[0][1] * _y_2)) + current_center_mass_X;
    double _y2_ = ((matrix_angles[1][0] * _x_2) + (matrix_angles[1][1] * _y_2)) + current_center_mass_Y;

    cout << _x1_ << " " << _y1_ << " " << _x2_ << " " << _y2_ << endl;

    if ((_x1_ < 0 || _x2_ < 0) && discriminant >= 0)
        collision = false;

    // glm::vec2 upmost = (vertices_slope[1] > vertices_slope[4]) ? glm::vec2(vertices_slope[0], vertices_slope[1]) : glm::vec2(vertices_slope[3], vertices_slope[4]);
    // glm::vec2 bottommost = (vertices_slope[1] < vertices_slope[4]) ? glm::vec2(vertices_slope[0], vertices_slope[1]) : glm::vec2(vertices_slope[3], vertices_slope[4]);
    // glm::vec2 rightmost = (vertices_slope[0] > vertices_slope[3]) ? glm::vec2(vertices_slope[0], vertices_slope[1]) : glm::vec2(vertices_slope[3], vertices_slope[4]);
    // glm::vec2 leftmost = (vertices_slope[0] < vertices_slope[3]) ? glm::vec2(vertices_slope[0], vertices_slope[1]) : glm::vec2(vertices_slope[3], vertices_slope[4]);
    //
    // if (_x1_ < leftmost.x && _x2_ < leftmost.x && _x1_ > rightmost.x && _x2_ > rightmost.x && discriminant >= 0)
    //   collision = false;
    // if (_y1_ < bottommost.y && _y2_ < bottommost.y && _y1_ > rightmost.y && _y2_ > rightmost.y && discriminant >= 0)
    //   collision = false;

    float point_middle_x = (_x1_ + _x2_) / 2.0f;
    float point_middle_y = (_y1_ + _y2_) / 2.0f;

    middle.push_back(point_middle_x);
    middle.push_back(point_middle_y);

    return middle;
}

void PointContact::superposition(float current_center_mass_X, float current_center_mass_Y, float vertices_slope[], float theta)
{
    vector<float> point_middle = point_collision(current_center_mass_X, current_center_mass_Y, vertices_slope, theta);
    float pA1 = (1 / (sqrt(1 + (point_middle[0] * point_middle[0]))));
    float pA2 = -(1 / (sqrt(1 + (point_middle[0] * point_middle[0]))));

    float sA1 = -point_middle[0] * pA1;
    float sA2 = -point_middle[0] * pA2;

    float xA1 = sA1 * draw.a;
    float yA1 = pA1 * draw.b;
    float xA2 = sA2 * draw.a;
    float yA2 = pA2 * draw.b;

    double xA1_ = ((matrix_angles[0][0] * xA1) + (matrix_angles[0][1] * yA1)) + current_center_mass_X;
    double yA1_ = ((matrix_angles[1][0] * xA1) + (matrix_angles[1][1] * yA1)) + current_center_mass_Y;
    double xA2_ = ((matrix_angles[0][0] * xA2) + (matrix_angles[0][1] * yA2)) + current_center_mass_X;
    double yA2_ = ((matrix_angles[1][0] * xA2) + (matrix_angles[1][1] * yA2)) + current_center_mass_Y;

    double A = -(vertices_slope[4] - vertices_slope[1]) / (vertices_slope[3] - vertices_slope[0]);
    double B = 1;

    double C = (-A * vertices_slope[3]) - vertices_slope[4];

    float per_1 = ((A * xA1_) + (B * yA1_) + C) / (sqrt((A * A) + (B * B)));
    float per_2 = ((A * xA2_) + (B * yA2_) + C) / (sqrt((A * A) + (B * B)));

    perpendicular.first = per_1;
    perpendicular.second = per_2;
}

#endif