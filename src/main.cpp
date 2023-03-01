#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;
typedef unsigned int uint;

// settings
const uint SCR_WIDTH = 2000;
const uint SCR_HEIGHT = 1000;

const float pi = 3.1415;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
float escala_radio(float radio, float escala);
float radianes(float angulo);
float angulo(float radianes);
float convert_new_coor_x(float x, float y, double grados);
float convert_new_coor_y(float x, float y, int grados);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
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

    /*****************************************************************************************************/

    int a; //radio mayor
    int b; //radio menos
    const int escala = 50;
    float origen_x, origen_y;

    cout<<"Ingrese el radio mayor: "; cin>>a;
    cout<<"Ingrese el radio menor: "; cin>>b;
    //cout<<"Ingrese una escala para la conversion: "; cin>> escala;
    cout<<"Ingrese la coordenada x de origen: "; cin>>origen_x;
    cout<<"Ingrese la coordenada y de origen: "; cin>>origen_y;

    // coordenada x , coordenada y , coordenada z = 0
    
    const float escala_x = escala_radio(a, escala);
    const float escala_y = escala_radio(b, escala);

    const int salto_angulos_draw = 10;
    const int n_triangulos = 360 / salto_angulos_draw;
    //const int pendiente = 2*pi / n_triangulos;

    const int size_vertices = 3 * (n_triangulos + 1);
    float vertices[size_vertices] = { origen_x , origen_y , 0.0f}; //coordenada de origen 

    const int size_index = 3 * n_triangulos;
    uint indices[size_index];

    int index_vertices = 3, index_indices = 0, index_indices_value = 1;
    double inclinacion = 0;

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

    //giro -> 2°
    //ademas 
    float vertices_giro[size_vertices];
    int grados_giro = 2;
    int grado_inicio = 0;

    for(int grado = grado_inicio ; grado < 360 ; grado+=grados_giro){
        vertices_giro[0] = 0.0f;
        vertices_giro[1] = 0.0f;
        vertices_giro[2] = 0.0f;

        int index_vertices_giro = 3;
        int index_ellipse_origen_x = 3 , index_ellipse_origen_y = 4;

        for(int j=0 ; j<n_triangulos ; j++){
            vertices_giro[index_vertices_giro] = convert_new_coor_x(vertices[index_ellipse_origen_x],vertices[index_ellipse_origen_y], grado);
            index_vertices_giro++;
            vertices_giro[index_vertices_giro] = convert_new_coor_y(vertices[index_ellipse_origen_x],vertices[index_ellipse_origen_y], grado);
            index_vertices_giro++;  
            vertices_giro[index_vertices_giro] = 0.0f;
            index_vertices_giro++;
            index_ellipse_origen_x+=3;
            index_ellipse_origen_y+=3;
        }

        /*cout<<"\n\n\n";

        for(int k=0 ; k < 3*(n_triangulos+1) ; k++){
            cout<<vertices_giro[k]<<" ";
        }*/
    }

    /*****************************************************************************************************/

    uint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_giro), vertices_giro, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, size_index, GL_UNSIGNED_INT, 0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        cout<<"si fue presionado";
    }
}