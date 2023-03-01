#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// settings
const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 1000;

const float pi = 3.1415;

void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

float escala_radio(float radio, float escala){
    cout<<radio/escala<<endl;
    return radio/escala;
}

float radianes(float angulo){
    return (angulo*pi)/180;
}

float angulo(float radianes){
    return (radianes*180)/pi;
}

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

    int a; //radio mayor
    int b; //radio menos
    const int escala = 50;

    cout<<"Ingrese el radio mayor: "; cin>>a;
    cout<<"Ingrese el radio menor: "; cin>>b;
    //cout<<"Ingrese una escala para la conversion: "; cin>> escala;

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
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

    // coordenada x , coordenada y , coordenada z = 0
    
    const float escala_x = escala_radio(a, escala);
    const float escala_y = escala_radio(b, escala);

    const int salto_angulos = 10;
    const int n_triangulos = 360 / salto_angulos;
    //const int pendiente = 2*pi / n_triangulos;

    const int size_vertices = 3 * (n_triangulos + 1);
    float vertices[size_vertices] = { 0.0f , 0.0f , 0.0f}; //coordenada de origen 

    const int size_index = 3 * n_triangulos;
    unsigned int indices[size_index];

    int index_vertices = 3, index_indices = 0, index_indices_value = 1;
    double inclinacion = 0;

    //llenar array de vertices (x,y,z=0)
    for(int i = 0; i < n_triangulos; i++){
        inclinacion = inclinacion + salto_angulos;
        vertices[index_vertices] = float(escala_x * cos(radianes(inclinacion)));  //coordenada X
        index_vertices++;
        vertices[index_vertices] = float(escala_y * sin(radianes(inclinacion)));  //coordenada Y
        index_vertices++; 
        vertices[index_vertices] = 0.0f;               //coordenada Z
        index_vertices++;
    }

    for(int i = 0; i < (n_triangulos+1)*3; i+=3){
        cout<<vertices[i]<<" "<<vertices[i+1]<<" "<<vertices[i+2]<<endl;
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


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while (!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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

