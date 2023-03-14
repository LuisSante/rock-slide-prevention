#include <iostream>
#include <math.h>
#include <chrono> // para la medición del tiempo
#include <thread> // para la gestión de hilos
#include <cstdio>
#include <cstring>

#include "draw.hpp"
#include "intersection.hpp"
#include "speed.hpp"
#include "f_normal.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// settings
const unsigned int SCR_WIDTH = 4000;
const unsigned int SCR_HEIGHT = 4000;

// control de velocidad de ejecución
double lastTime = glfwGetTime();
double deltaTime = 0.0;
double desiredFPS = 60.0;
double frameTime = 1.0 / desiredFPS;

void processInput(GLFWwindow *window)
{
    // float deltaTime{ getDeltaTime() };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void transformVertices(float *vertices, int numVertices, glm::mat4 transform)
{
    for (int i = 0; i < numVertices; i += 3)
    {
        glm::vec4 transformedVertex = transform * glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
        vertices[i] = transformedVertex.x;
        vertices[i + 1] = transformedVertex.y;
        vertices[i + 2] = transformedVertex.z;
    }
}

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "uniform mat4 transform; \n"
                                 "out vec3 ourColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = transform * vec4(aPos, 1.0);\n"
                                 "   ourColor = aColor;\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0f);\n"
                                   "}\n\0";

                    
const char *vertexShaderSource_talud = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                       "}\0";

const char *fragmentShaderSource_talud = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
                                         "}\n\0";

const char *vertexShaderSource_grid = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                       "}\0";

const char *fragmentShaderSource_grid = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                         "}\n\0";
int main()
{
    float escala = 3500;
    float origen_x = 0, origen_y = 0;
    float Xd = origen_x / escala;
    float Yd = origen_y / escala;

    float radio_mayor = 200, radio_menor = 150;
    float a = radio_mayor / escala, b = radio_menor / escala;
    float vertices[57]; // coordenada de origen
    unsigned int indices[54];

    /*runge kutta*/
    float vx = 155.8844625;
    float vy = 90;
    float theta = 0.0 * 3.14 / 180.0;
    float dt = 0.01;
    const int n = 21;
    float delta_pos_x[n] = {0};
    float delta_pos_y[n] = {0};
    float theta_array[n] = {0};

    float vertices_talud[6] = {0.5, 12, 0, 12.5, 1, 0};
    Draw elipse1(Xd, Yd, a , b, vx, vy, theta, dt, n);
    Intersection inter(elipse1);

    /*coordenadas del talud*/
    const int size_coor_grid = 4;
    float talud[size_coor_grid * 3];
    unsigned int indices_grid[size_coor_grid * 3];

    /*calculo del punto medio*/
    /*float inversa[MAX_DIMENSION][MAX_DIMENSION];
    float ml, bl;
    float punto_medio_x, punto_medio_y;*/

    float vertices_grid[] = {
        -1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,

        -1.0f, 0.5f, 0.0,
        1.0f, 0.5f, 0.0,

        0.5f, -1.0f, 0.0,
        0.5f, 1.0f, 0.0,

        0.0f, -1.0f, 0.0,
        0.0f, 1.0f, 0.0,
        
        -0.5f, -1.0f, 0.0,
        -0.5f, 1.0f, 0.0,

        -1.0f, -0.5f, 0.0,
        1.0f, -0.5f, 0.0,};


    /*Superposition super(elipse1, ml, bl);
    Speed speed;
    FuerzaNormal fn;*/

    elipse1.vertices_elipse(vertices);
    elipse1.indices_elipse(indices);
    elipse1.move(delta_pos_x, delta_pos_y, theta_array);

    for (int i = 0; i < n; i++)
    {
        cout << delta_pos_x[i] * 3500 << " " << delta_pos_y[i] * 3500<< " " << theta_array[i]<< endl;
    }

    /*super.superposition(ml, bl, vertices_talud);
    speed.velocidad(vertices_talud, punto_medio_x, punto_medio_y);

    fn.modulo();*/

    /******************************************************************************************************************************/

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /******************************************************************************************************/

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /******************************************************************************************************/

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    /******************************************************************************************************/
    unsigned int vertexShader_grid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader_grid, 1, &vertexShaderSource_grid, NULL);
    glCompileShader(vertexShader_grid);

    int success_grid;
    char infoLog_grid[512];
    glGetShaderiv(vertexShader_grid, GL_COMPILE_STATUS, &success_grid);
    if (!success_grid)
    {
        glGetShaderInfoLog(vertexShader_grid, 512, NULL, infoLog_grid);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog_grid << std::endl;
    }

    unsigned int fragmentShader_grid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_grid, 1, &fragmentShaderSource_grid, NULL);
    glCompileShader(fragmentShader_grid);

    glGetShaderiv(fragmentShader_grid, GL_COMPILE_STATUS, &success_grid);
    if (!success_grid)
    {
        glGetShaderInfoLog(fragmentShader_grid, 512, NULL, infoLog_grid);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog_grid << std::endl;
    }

    unsigned int shaderProgram_grid = glCreateProgram();
    glAttachShader(shaderProgram_grid, vertexShader_grid);
    glAttachShader(shaderProgram_grid, fragmentShader_grid);
    glLinkProgram(shaderProgram_grid);

    glGetProgramiv(shaderProgram_grid, GL_LINK_STATUS, &success_grid);
    if (!success_grid)
    {
        glGetProgramInfoLog(shaderProgram_grid, 512, NULL, infoLog_grid);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog_grid << std::endl;
    }
    glDeleteShader(vertexShader_grid);
    glDeleteShader(fragmentShader_grid);

    /******************************************************************************************************/

    unsigned int VBO_grid, VAO_grid;
    glGenVertexArrays(1, &VAO_grid);
    glGenBuffers(1, &VBO_grid);
    glBindVertexArray(VAO_grid);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_grid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_grid), vertices_grid, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /******************************************************************************************************/

    unsigned int vertexShader_talud = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader_talud, 1, &vertexShaderSource_talud, NULL);
    glCompileShader(vertexShader_talud);

    int success_talud;
    char infoLog_talud[512];
    glGetShaderiv(vertexShader_grid, GL_COMPILE_STATUS, &success_talud);
    if (!success_talud)
    {
        glGetShaderInfoLog(vertexShader_talud, 512, NULL, infoLog_talud);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog_talud << std::endl;
    }

    unsigned int fragmentShader_talud = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_talud, 1, &fragmentShaderSource_talud, NULL);
    glCompileShader(fragmentShader_talud);

    glGetShaderiv(fragmentShader_talud, GL_COMPILE_STATUS, &success_talud);
    if (!success_talud)
    {
        glGetShaderInfoLog(fragmentShader_talud, 512, NULL, infoLog_talud);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog_talud << std::endl;
    }

    unsigned int shaderProgram_talud = glCreateProgram();
    glAttachShader(shaderProgram_talud, vertexShader_talud);
    glAttachShader(shaderProgram_talud, fragmentShader_talud);
    glLinkProgram(shaderProgram_talud);

    glGetProgramiv(shaderProgram_talud, GL_LINK_STATUS, &success_talud);
    if (!success_talud)
    {
        glGetProgramInfoLog(shaderProgram_talud, 512, NULL, infoLog_talud);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog_talud << std::endl;
    }
    glDeleteShader(vertexShader_talud);
    glDeleteShader(fragmentShader_talud);


    unsigned int VBO_talud, VAO_talud;
    glGenVertexArrays(1, &VAO_talud);
    glGenBuffers(1, &VBO_talud);
    glBindVertexArray(VAO_talud);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_talud);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_talud), vertices_talud, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /******************************************************************************************************/

    int pos = 1;
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glUseProgram(shaderProgram);

        // render

        glm::mat4 transform = glm::mat4(1.0f);
        if (pos < n)
        {
            cout << "angle: " << theta_array[pos] << endl;

            float traslate_X = delta_pos_x[pos];
            float traslate_Y = delta_pos_y[pos];
            float angle = theta_array[pos];
            transform = glm::translate(transform, glm::vec3(traslate_X, traslate_Y, 0.0f));
            transform = glm::rotate(transform, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));
            //traslate_X = 0;
            //traslate_Y = 0;

            float outvertices[57];
            memcpy(outvertices, vertices, sizeof(vertices));

            transformVertices(outvertices, 57, transform);

            for (int i = 0; i < 57; i += 3)
            {
                cout << "Vertex " << i / 3 << ": (" << outvertices[i] * 3500<< ", " << outvertices[i + 1] * 3500<< ", " << outvertices[i + 2] * 3500 << ")" << std::endl;
            }

            pos++;
        }

        else{
            float traslate_X = delta_pos_x[n-1];
            float traslate_Y = delta_pos_y[n-1];
            float angle = theta_array[n-1];
            transform = glm::translate(transform, glm::vec3(traslate_X, traslate_Y, 0.0f));
            transform = glm::rotate(transform, (float)angle, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 57, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram_grid);
        glBindVertexArray(VAO_grid);
        glDrawArrays(GL_LINES, 0, 100);

        glUseProgram(shaderProgram_talud);
        glBindVertexArray(VAO_talud);
        glDrawArrays(GL_LINES, 0, 100);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glDeleteVertexArrays(1, &VAO_grid);
    glDeleteBuffers(1, &VBO_grid);
    glDeleteProgram(shaderProgram_grid);

    glfwTerminate();

    return 0;
}