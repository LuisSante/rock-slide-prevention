/*****************************************************************************/

#include <HSGIL/hsgil.hpp>

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <cmath>
#include <cstdio>
#include <cstring>

#include "draw.hpp"
#include "Rk.hpp"
#include "punto_contacto.hpp"
#include "speed_f_normal.hpp"

using std::vector;

// Window Size Settings
constexpr unsigned int SCR_WIDTH = 4000;
constexpr unsigned int SCR_HEIGHT = 4000;

constexpr unsigned int SIZE_COORD_GRID = 4;

constexpr unsigned int NUMBER_OF_SECTIONS = 18;
constexpr unsigned int ROCK_VERTEX_DATA_SIZE = (NUMBER_OF_SECTIONS + 1) * 6;
constexpr unsigned int ROCK_INDICES_SIZE = NUMBER_OF_SECTIONS * 3;

/*****************************************************************************/
void transformVertices(float* vertices, int numVertices, const glm::mat4& transform)
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
    gil::Shader taludShader("talud");
    gil::Shader gridShader("grid");

    std::ofstream output("C:/Users/Usuario/Desktop/hilarios/src/report.txt");
    /*****************************************************************************/

    // Initial Setup Stuff
    /*****************************************************************************/
    // General Scale
    float scale = 3500.0f;

    // Non-sacled origin => Scaled origin
    float origen_x = 0.0f;
    float origen_y = 0.0f;
    float Xd = origen_x / scale;
    float Yd = origen_y / scale;

    // Non-scaled radius => Scaled radius
    float radio_mayor = 20.0f;
    float radio_menor = 15.0f;
    float a = radio_mayor / scale;
    float b = radio_menor / scale;

    // Rock Computing Utils [Runge Kutta]
    float vx = 155.9f;
    float vy = 90.0f;
    float theta = 0.0f * M_PI / 180.0f;
    Draw elipse(Xd, Yd, a, b, NUMBER_OF_SECTIONS);
    /*****************************************************************************/

    // Raw Vertex Data
    /*****************************************************************************/
    // Rock raw data
    float rock_vertex_data[ROCK_VERTEX_DATA_SIZE];
    unsigned int rock_indices[ROCK_INDICES_SIZE];
    elipse.vertices_elipse(rock_vertex_data);
    elipse.indices_elipse(rock_indices);

    // Talud raw data
    float grid_vertex_data[] = {
        -1.0f,  0.0f, 0.0f,     1.0f,  0.0f, 0.0f,
        -1.0f,  0.5f, 0.0f,     1.0f,  0.5f, 0.0f,
         0.5f, -1.0f, 0.0f,     0.5f,  1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,     0.0f,  1.0f, 0.0f,
        -0.5f, -1.0f, 0.0f,    -0.5f,  1.0f, 0.0f,
        -1.0f, -0.5f, 0.0f,     1.0f, -0.5f, 0.0f
    };

    // Talud raw data
    float talud[SIZE_COORD_GRID * 3];
    float talud_vertex_data[6] = {
        -1.0f, -0.02f, 0.0f, 1.0f, -0.02f, 0.0f
    };

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    unsigned int VAO_talud = 0;
    unsigned int VBO_talud = 0;

    glGenVertexArrays(1, &VAO_talud);
    glGenBuffers(1, &VBO_talud);

    glBindVertexArray(VAO_talud);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_talud);
    glBufferData(GL_ARRAY_BUFFER, sizeof(talud_vertex_data), talud_vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*****************************************************************************/

    vector<float> x_array;
    vector<float> y_array;
    vector<float> vx_array;
    vector<float> vy_array;
    vector<float> theta_array;

    PuntoContacto inter(elipse);
    Speed_F_Normal speed(elipse);

    RungeKutta rk(vx, vy, theta, elipse, speed);
    rk.runge_kutta(x_array, y_array, vx_array, vy_array, theta_array);

    int n = vx_array.size();

    int pos = 1;
    //gil::Timer timer(true);
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
        glm::mat4 transform = glm::mat4(1.0f);
        if (pos < n)
        {
            output << "Angle: " << theta_array[pos] << endl;

            float traslate_X = x_array[pos] / scale;
            float traslate_Y = y_array[pos] / scale;
            float angle = theta_array[pos];
            transform = glm::translate(transform, glm::vec3(traslate_X, traslate_Y, 0.0f));
            transform = glm::rotate(transform, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));

            // Impresion de vertices para reporte
            // Rescatar el centro de masa
            float outvertices[ROCK_VERTEX_DATA_SIZE];
            memcpy(outvertices, rock_vertex_data, sizeof(rock_vertex_data));

            transformVertices(outvertices, NUMBER_OF_SECTIONS, transform);
            for (int i = 0; i < NUMBER_OF_SECTIONS + 1; ++i)
            {
                output << "Vertex " << i << ": (" <<
                (outvertices[i * 6 + 0] * scale) << ", " <<
                (outvertices[i * 6 + 1] * scale) << ", " <<
                (outvertices[i * 6 + 2] * scale) << ")" << std::endl;
            }

            // Evaluar cada movimiento para ver si esta viajando
            // funcion(rescatar centro de masa)
            float centro_masa_X = outvertices[0];
            float centro_masa_Y = outvertices[1];
            float current_velocity_X = vx_array[pos]; 
            float current_velocity_Y = vy_array[pos];
            //cout << centro_masa_X * scale << " " << centro_masa_Y * scale << endl;

            inter.superposition(centro_masa_X , centro_masa_Y , talud_vertex_data);
            if (inter.collision == true)
            {
                cout<<current_velocity_X << " "<<current_velocity_Y << " " << angle<< endl;
                // Calcula las fuerzas para el rebote
                speed.momentos(centro_masa_X , centro_masa_Y , current_velocity_X , current_velocity_Y , angle, talud_vertex_data);
                return 0;

                // Iran las funciones que haran que la elipse rebote
                
                //calcular de nuevo Rk
                /*while(current_velocity_X != 0 && current_velocity_Y != 0)
                {
                    
                }*/
            }

            // Incremento para ver fotogramas
            pos+=1;
        }
        else
        {
            float traslate_X = x_array[n - 1] / scale;
            float traslate_Y = y_array[n - 1] / scale;
            float angle = theta_array[n - 1];
            transform = glm::translate(transform, glm::vec3(traslate_X, traslate_Y, 0.0f));
            transform = glm::rotate(transform, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));
        }

        gridShader.use();
        glBindVertexArray(VAO_grid);
        glDrawArrays(GL_LINES, 0, 100);

        taludShader.use();
        glBindVertexArray(VAO_talud);
        glDrawArrays(GL_LINES, 0, 100);

        rockShader.use();
        rockShader.setMat4("transform", transform);
        glBindVertexArray(VAO_rock);
        glDrawElements(GL_TRIANGLES, ROCK_VERTEX_DATA_SIZE, GL_UNSIGNED_INT, (const void*)0);

        // Post Tick Calls
        window.swapBuffers();
        //timer.tick();
    }

    glDeleteVertexArrays(1, &VAO_rock);
    glDeleteBuffers(1, &VBO_rock);
    glDeleteBuffers(1, &EBO_rock);

    glDeleteVertexArrays(1, &VAO_grid);
    glDeleteBuffers(1, &VBO_grid);

    glDeleteVertexArrays(1, &VAO_talud);
    glDeleteBuffers(1, &VBO_talud);

    return 0;
}
