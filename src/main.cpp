/*****************************************************************************/

#include <HSGIL/hsgil.hpp>
#include <ecs.hpp>

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <utility>

#include <cmath>
#include <cstdio>
#include <cstring>

#include "draw.hpp"
#include "move.hpp"
#include "collision.hpp"
#include "speed_f_normal.hpp"

using std::vector;
std::ofstream report("C:/Users/Usuario/Desktop/rock_slide/src/reports/report.txt");
std::ofstream rebound("C:/Users/Usuario/Desktop/rock_slide/src/reports/report_rebound.txt");
std::ofstream Frame_Rk("C:/Users/Usuario/Desktop/rock_slide/src/reports/frame_RK.txt");
std::ofstream Rocks_Out("C:/Users/Usuario/Desktop/rock_slide/src/reports/Rocks_Out.txt");
std::ofstream module("C:/Users/Usuario/Desktop/rock_slide/src/reports/module.txt");

// Window Size Settings
constexpr unsigned int SCR_WIDTH = 1000;
constexpr unsigned int SCR_HEIGHT = 1000;

// constexpr unsigned int SIZE_COORD_GRID = 4;

constexpr unsigned int NUMBER_OF_SECTIONS = 18;
constexpr unsigned int ROCK_VERTEX_DATA_SIZE = (NUMBER_OF_SECTIONS + 1) * 6;
constexpr unsigned int ROCK_INDICES_SIZE = NUMBER_OF_SECTIONS * 3;

// Seed
constexpr int DATA = 997;
constexpr long long B = 314159239;
constexpr long long C = 453806245;
constexpr int init = 36;
constexpr double mean = 0.25;
constexpr double standard_deviation = 0.05;

/*****************************************************************************/
void transformVertices(float *vertices, int numVertices, const glm::mat4 &transform)
{
    for (int i = 0; i < numVertices + 1; ++i)
    {
        glm::vec4 transformedVertex = transform * glm::vec4(vertices[i * 6 + 0],
                                                            vertices[i * 6 + 1],
                                                            vertices[i * 6 + 2],
                                                            1.0f);
        vertices[i * 6 + 0] = transformedVertex.x;
        vertices[i * 6 + 1] = transformedVertex.y;
        vertices[i * 6 + 2] = transformedVertex.z;
    }
}

void print_rk(float t, float center_mass_x, float center_mass_y, float vx, float vy, float theta, Speed_F_Normal speed)
{
    Frame_Rk << "T: " << t << " (center_mass_x,center_mass_y): (" << center_mass_x << " , " << center_mass_y << ") vx: " << vx << " vy: " << vy << " giro (rads): " << theta << " Mg: " << speed.M_G << " F_normal: (" << speed.F_normal.first << " , " << speed.F_normal.second << ") F_tangential : (" << speed.F_tangential.first << " , " << speed.F_tangential.second << ")" << endl;
}

void print_vertex(float out_vertex[], float theta)
{
    report << theta << endl;
    for (int i = 0; i < NUMBER_OF_SECTIONS + 1; ++i)
    {
        report << "Vertex " << i << ": (" << (out_vertex[i * 6 + 0]) << ", " << (out_vertex[i * 6 + 1]) << ", " << (out_vertex[i * 6 + 2]) << ")" << endl;
    }
    report << endl
           << endl;
}

void print_module(float t, float velocity_x, float velocity_y)
{
    float vel_x = velocity_x * velocity_x;
    float vel_y = velocity_y * velocity_y;

    float mod = sqrt(vel_x + vel_y);

    module << "t : " << t << " modulo : " << mod << endl;
}

void print_report(float t, float out_vertex[], PointContact point, Speed_F_Normal speed)
{
    rebound << " Second: " << t << "\t\t\t\t\t (X,Y): (" << out_vertex[0] << "," << out_vertex[1] << ") \t Sig_n: (" << point.perpendicular.first << "\t" << point.perpendicular.second << ") \t D_Sig_n : (" << speed.velocity_sigma[2] << " i " << speed.velocity_sigma[3] << " j) \t D_Sig_t: (" << speed.velocity_sigma[0] << " i " << speed.velocity_sigma[1] << " j) \t\t F_normal : " << speed.FN << "\t (" << speed.F_normal.first << " i, " << speed.F_normal.second << " j) \t F_tangential: " << speed.Ft << " \t (" << speed.F_tangential.first << "i , " << speed.F_tangential.second << ") \t"
            << "\t Mg : " << speed.M_G << endl;
}

/*****************************************************************************/
int main()
{
    // Window Setup
    /*****************************************************************************/
    gil::RenderingWindow window(SCR_WIDTH, SCR_HEIGHT, "Rendering");
    if (!window.isReady())
    {
        return -1;
    }

    gil::InputHandler inputHandler;
    window.setInputHandler(inputHandler);

    gil::Shader rockShader("rock");
    gil::Shader slopeShader("slope");
    gil::Shader gridShader("grid");

    // std::ofstream report("../src/reports/report.txt");
    // std::ofstream rebound("../src/reports/report_rebound.txt");
    // std::ofstream Frame_Rk("../src/reports/frame_RK.txt");
    /*****************************************************************************/

    // Initial Setup Stuff
    /*****************************************************************************/
    // General Scale
    float scale = 15.0f;

    float center_mass_x = 0.55f;
    float center_mass_y = 13.0f;

    float a = 0.55f;
    float b = 0.4f;

    // Rock Computing Utils [RK]
    float vx = 1.0f;
    float vy = 0.0f;
    float theta = 0.0f * M_PI / 180.0f;
    Draw ellipse(a, b, NUMBER_OF_SECTIONS);
    /*****************************************************************************/

    // Raw Vertex Data
    /*****************************************************************************/
    // Rock raw data
    float rock_vertex_data[ROCK_VERTEX_DATA_SIZE];
    unsigned int rock_indices[ROCK_INDICES_SIZE];
    ellipse.vertex_ellipse(rock_vertex_data);
    ellipse.index_ellipse(rock_indices);

    // slope raw data
    float grid_vertex_data[] = {
        -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f,
        0.5f, -1.0f, 0.0f, 0.5f, 1.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -1.0f, 0.0f, -0.5f, 1.0f, 0.0f,
        -1.0f, -0.5f, 0.0f, 1.0f, -0.5f, 0.0f};

    // slope raw data
    // float slope[SIZE_COORD_GRID * 3];

    float slope_vertex_data[6] = {
        /*3.05f, 0.0f, 0.0f,
        0.0f, 12.20f, 0.0f,*/

        0.0f, 12.20f, 0.0f,
        3.05f, 0.0f, 0.0f

        /*3.05f, 0.0f, 0.0f,
        13.0f, 2.4888, 0.0f*/
    };

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::ortho(-scale, scale, -scale, scale, -1.0f, 1.0f);

    /*****************************************************************************/

    unsigned int VAO_rock = 0;
    unsigned int VBO_rock = 0;
    unsigned int EBO_rock = 0;

    glGenVertexArrays(1, &VAO_rock);
    glGenBuffers(1, &VBO_rock);
    glGenBuffers(1, &EBO_rock);

    glBindVertexArray(VAO_rock);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_rock);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rock_vertex_data), rock_vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_rock);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rock_indices), rock_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*****************************************************************************/

    unsigned int VAO_grid = 0;
    unsigned int VBO_grid = 0;

    glGenVertexArrays(1, &VAO_grid);
    glGenBuffers(1, &VBO_grid);

    glBindVertexArray(VAO_grid);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_grid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertex_data), grid_vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    unsigned int VAO_slope = 0;
    unsigned int VBO_slope = 0;

    glGenVertexArrays(1, &VAO_slope);
    glGenBuffers(1, &VBO_slope);

    glBindVertexArray(VAO_slope);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_slope);
    glBufferData(GL_ARRAY_BUFFER, sizeof(slope_vertex_data), slope_vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    constexpr float tf = 30.0f;
    static const float h = 0.01f;
    float MG = 0.0f;
    float w = 0.2f;

    int i = 0;
    float t = 0.0f;

    PointContact point(ellipse);
    Speed_F_Normal speed(ellipse);

    Move rk(ellipse);

    gil::Timer timer;
    while (window.isActive())
    {
        // Pre Tick calls
        window.pollEvents();
        if (inputHandler.onKeyReleased(gil::KEY_ESCAPE))
        {
            window.close();
            continue;
        }

        // OpenGL clearing functions
        glClearColor(0.2f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering Stuff
        glm::mat4 model_rock_shader = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);

        slopeShader.use();
        slopeShader.setMat4("model", model);
        slopeShader.setMat4("view", view);
        slopeShader.setMat4("proj", proj);
        glBindVertexArray(VAO_slope);
        glDrawArrays(GL_LINES, 0, 100);

        gridShader.use();
        glBindVertexArray(VAO_grid);
        glDrawArrays(GL_LINES, 0, 100);

        t = i * h;
        print_rk(t, center_mass_x, center_mass_y, vx, vy, theta, speed);
        print_module(t, vx, vy);
        rk.movement(vx, vy, center_mass_x, center_mass_y, theta, w, speed.M_G, h, t, i, speed.F_normal, speed.F_tangential);

        model_rock_shader = glm::translate(model_rock_shader, glm::vec3(center_mass_x, center_mass_y, 0.0f));
        model_rock_shader = glm::rotate(model_rock_shader, (float)theta, glm::vec3(0.0f, 0.0f, 1.0f));

        float out_vertex[ROCK_VERTEX_DATA_SIZE];
        memcpy(out_vertex, rock_vertex_data, sizeof(rock_vertex_data));
        transformVertices(out_vertex, NUMBER_OF_SECTIONS, model_rock_shader);

        float angle_degrees = theta * 180 / 3.14;

        print_vertex(out_vertex, theta);

        point.superposition(out_vertex[0], out_vertex[1], slope_vertex_data, angle_degrees);

        if (point.collision == true)
        {
            speed.momentos(out_vertex[0], out_vertex[1], vx, vy, angle_degrees, w, h, slope_vertex_data);
            print_report(t, out_vertex, point, speed);
            point.collision = false;
            // window.close();
            // continue;
        }

        i++;

        rockShader.use();
        rockShader.setMat4("model", model_rock_shader);
        rockShader.setMat4("view", view);
        rockShader.setMat4("proj", proj);
        glBindVertexArray(VAO_rock);
        glDrawElements(GL_TRIANGLES, ROCK_VERTEX_DATA_SIZE, GL_UNSIGNED_INT, (const void *)0);

        // Post Tick Calls
        window.swapBuffers();
        timer.tick();

        // std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    glDeleteVertexArrays(1, &VAO_rock);
    glDeleteBuffers(1, &VBO_rock);
    glDeleteBuffers(1, &EBO_rock);

    glDeleteVertexArrays(1, &VAO_grid);
    glDeleteBuffers(1, &VBO_grid);

    glDeleteVertexArrays(1, &VAO_slope);
    glDeleteBuffers(1, &VBO_slope);

    EntityManager<RockEntity> rockEntityManager;
    for (int i = 0; i < 1000; ++i)
    {
        RockEntity *rock = rockEntityManager.CreateEntity();
        rock->Init(1.0f, 1.0f, Rocks_Out);
    }
    rockEntityManager.PrintEntities();

    return 0;
}