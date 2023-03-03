#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
typedef unsigned int uint;

// settings
const uint SCR_WIDTH = 2000;
const uint SCR_HEIGHT = 1000;

const float pi = 3.1415;
//const float g = 9.81;

const int salto_angulos_draw = 10;
const int n_triangulos = 360 / salto_angulos_draw;
//const int pendiente = 2*pi / n_triangulos;

const int size_vertices = 3 * (n_triangulos + 1);
const int size_index = 3 * n_triangulos;

float vertices[size_vertices]; //coordenada de origen 
uint indices[size_index];

const int size_coor_talud = 4;

float talud[size_coor_talud*3];
uint indices_talud[size_coor_talud*3];

//float vertices_giro[size_vertices];

int index_vertices = 3, index_indices = 0, index_indices_value = 1;
double inclinacion = 0;
glm::mat4 transform = glm::mat4(1.0f);

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float escala_radio(float radio, float escala);
float radianes(float angulo);
float angulo(float radianes);
float convert_new_coor_x(float x, float y, double grados);
float convert_new_coor_y(float x, float y, int grados);

double f(double t, double x, double y, double vx, double vy);
double g(double t, double x, double y, double vx, double vy);
void rungeKutta(double t0, double x0, double y0, double vx0, double vy0, double tf, double h, double *t, double *x, double *y, double *vx, double *vy, int n);

float getDeltaTime();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

/*const char *vertexShaderSource_talud = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource_talud = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";*/

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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


    //codigo

    /*******/

    int a = 20; //radio mayor
    int b = 15; //radio menor
    int escala = 100;
    float origen_x = 0, origen_y = 0;

    //cout<<"Ingrese el radio mayor: "; cin>>a;
    //cout<<"Ingrese el radio menor: "; cin>>b;
    //cout<<"Ingrese una escala para la conversion: "; cin>> escala;
    //cout<<"Ingrese la coordenada x de origen: "; cin>>origen_x;
    //cout<<"Ingrese la coordenada y de origen: "; cin>>origen_y;

    // coordenada x , coordenada y , coordenada z = 0
    
    const float escala_x = escala_radio(a, escala);
    const float escala_y = escala_radio(b, escala);

    vertices[0] = origen_x; 
    vertices[1] = origen_y;
    vertices[2] = 0.0f;

    //llenar array de vertices (x,y,z=0)
    for(int i = 0; i < n_triangulos; i++){
        inclinacion = inclinacion + salto_angulos_draw;
        vertices[index_vertices] = float(escala_x * cos(radianes(inclinacion)));  //coordenada X
        index_vertices++;
        vertices[index_vertices] = float(escala_y * sin(radianes(inclinacion)));  //coordenada Y
        index_vertices++; 
        vertices[index_vertices] = 0.0f;               //coordenada Z
        index_vertices++;
    }

    //llenar el array de indices
    for (int j = 1; j < n_triangulos; j++) {
        indices[index_indices] = 0;//todos los triangulos parten del centro
        index_indices++;
        indices[index_indices] = index_indices_value;//2do vertices
        index_indices++;
        index_indices_value++;
        indices[index_indices] = index_indices_value;//3er vertice
        index_indices++;
    }
    indices[index_indices] = index_indices_value;
    index_indices++;
    indices[index_indices] = 0;
    index_indices++;
    indices[index_indices] = 1;


    /**************************************************************************/    
    //generamos el vector de tiempo aqui -> para generar el vector de velocidades


    double t0 = 0.0; // tiempo inicial
    double x0 = 0.0; // posición inicial en el eje x
    double y0 = 0.0; // posición inicial en el eje y
    double vx0 = 155.9; // velocidad inicial en el eje x
    double vy0 = 90; // velocidad inicial en el eje y
    double tf = 20.0; // tiempo final
    double h = 1; // paso de integración

    int n = (tf - t0)/h + 1; // número de puntos

    double *t = new double[n]; // array para el tiempo
    double *x = new double[n]; // array para la posición en el eje x
    double *y = new double[n]; // array para la posición en el eje y
    double *vx = new double[n]; // array para la velocidad en el eje x
    double *vy = new double[n]; // array para la velocidad en el eje y

    rungeKutta(t0, x0, y0, vx0, vy0, tf, h, t, x, y, vx, vy, n); // llama a la función rungeKutta

    // imprime los resultados
    for (int i = 0; i < n; i++) {
        cout << "t = " << t[i] << " vx= " << vx[i] << "   x= " << x[i] << " vy = " << vy[i] << "   y = " << y[i] << endl;
    }

    //tiempo nos ayudará a iterar en la "tabla" y determinar el momento antes del impacto
    /**************************************************************************/

    uint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* ***************************************************************/
    float talud_x , talud_y;
    /*for(int k = 0 ; k < 3*size_coor_talud ; k+=3){
        cout<<"Ingrese el vertice de su talud en x: "; cin>>talud_x;
        cout<<"Ingrese el vertice de su talud en y: "; cin>>talud_y;
        talud[k] = float(talud_x/escala);
        talud[k+1] = float(talud_y/escala);
        talud[k+2] = 0.0f;
    }

    unsigned int VBO_talud;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_talud);
    glBufferData(GL_ARRAY_BUFFER, sizeof(talud), talud, GL_STATIC_DRAW);

    // Definir cómo se interpretan los datos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Dibujar las líneas
    glDrawArrays(GL_LINES, 0, size_coor_talud);/*

    /* ***************************************************************/

    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);


    /********************************************************************/

    /*uint vertexShader_talud = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader_talud, 1, &vertexShaderSource_talud, NULL);
    glCompileShader(vertexShader_talud);

    uint fragmentShader_talud = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_talud, 1, &fragmentShaderSource_talud, NULL);
    glCompileShader(fragmentShader_talud);

    uint shaderProgram_talud = glCreateProgram();
    glAttachShader(shaderProgram_talud, vertexShader_talud);
    glAttachShader(shaderProgram_talud, fragmentShader_talud);
    glLinkProgram(shaderProgram_talud);

    glDeleteShader(vertexShader_talud);
    glDeleteShader(fragmentShader_talud);

    glUseProgram(shaderProgram_talud);*/

    /*****************************************************************************************************/

    
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window);

		//render
        transform = glm::rotate(transform, (float)glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(0.0001f, 0.0001f, 0.0f));
        glUseProgram(shaderProgram);
        //transform
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        //glBindVertexArray(VBO_talud);

        glDrawElements(GL_TRIANGLES, size_index, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    
    // libera la memoria de los arrays
    delete[] t;
    delete[] x;
    delete[] y;
    delete[] vx;
    delete[] vy;
    return 0;
}

void processInput(GLFWwindow *window){
    //float deltaTime{ getDeltaTime() };
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        cout<<"si presiona ESPACIO"<<endl;
    }
}

float escala_radio(float radio, float escala){
    return radio/escala;
}

float radianes(float angulo){
    return (angulo*pi)/180;
}

float angulo(float radianes){
    return (radianes*180)/pi;
}

float convert_new_coor_x(float x, float y, double grados){
    return float(x*cos(radianes(grados)) - y*sin(radianes(grados)));
}

float convert_new_coor_y(float x, float y, int grados){
    return float(y*cos(radianes(grados)) + x*sin(radianes(grados)));
}

double f(double t, double x, double y, double vx, double vy) {
    double m = 120.0; // masa del objeto
    double F = 0.0; // fuerza neta que actúa sobre el objeto
    double ax = F/m; // aceleración en el eje x
    return ax; // devuelve la aceleración en el eje x
}

double g(double t, double x, double y, double vx, double vy) {
    double m = 120.0; // masa del objeto
    double g = 9.81;
    double F = m*g; // fuerza neta que actúa sobre el objeto
    double ay = -F/m; // aceleración en el eje y
    cout<<ay;
    return ay; // devuelve la aceleración en el eje y
}

void rungeKutta(double t0, double x0, double y0, double vx0, double vy0, double tf, double h, double *t, double *x, double *y, double *vx, double *vy, int n) {
    double k1x, k2x, k3x, k4x;
    double k1y, k2y, k3y, k4y;
    double k1vx, k2vx, k3vx, k4vx;
    double k1vy, k2vy, k3vy, k4vy;
    double ti = t0;
    double xi = x0;
    double yi = y0;
    double vxi = vx0;
    double vyi = vy0;
    
    for (int i = 0; i < n; i++) {
        t[i] = ti;
        x[i] = xi;
        y[i] = yi;
        vx[i] = vxi;
        vy[i] = vyi;
        
        k1x = h * vxi;
        k1y = h * vyi;
        k1vx = h * f(ti, xi, yi, vxi, vyi);
        k1vy = h * g(ti, xi, yi, vxi, vyi);
        
        k2x = h * (vxi + k1vx/2);
        k2y = h * (vyi + k1vy/2);
        k2vx = h * f(ti + h/2, xi + k1x/2, yi + k1y/2, vxi + k1vx/2, vyi + k1vy/2);
        k2vy = h * g(ti + h/2, xi + k1x/2, yi + k1y/2, vxi + k1vx/2, vyi + k1vy/2);
        
        k3x = h * (vxi + k2vx/2);
        k3y = h * (vyi + k2vy/2);
        k3vx = h * f(ti + h/2, xi + k2x/2, yi + k2y/2, vxi + k2vx/2, vyi + k2vy/2);
        k3vy = h * g(ti + h/2, xi + k2x/2, yi + k2y/2, vxi + k2vx/2, vyi + k2vy/2);

        k4x = h * (vxi + k3vx/2);
        k4y = h * (vyi + k3vy/2);
        k4vx = h * f(ti + h, xi + k3x, yi + k3y, vxi + k3vx, vyi + k3vy);
        k4vy = h * g(ti + h, xi + k3x, yi + k3y, vxi + k3vx, vyi + k3vy);

        ti += h;
        xi += (k1x + 2*k2x + 2*k3x + k4x)/6;
        yi += (k1y + 2*k2y + 2*k3y + k4y)/6;
        vxi += (k1vx + 2*k2vx + 2*k3vx + k4vx)/6;
        vyi += (k1vy + 2*k2vy + 2*k3vy + k4vy)/6;
    }
}

float getDeltaTime(){
    static float lastTime{ 0.0f };
    float currentTime{ static_cast<float>(glfwGetTime()) };
    float deltaTime{ currentTime - lastTime };
    lastTime = currentTime;
    return deltaTime;
}