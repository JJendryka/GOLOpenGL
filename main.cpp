#define SIMULATION_WIDTH 1024
#define SIMULATION_HEIGHT 1024

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* window;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>

#include "shader.hpp"
using namespace glm;


void mouseHandler(bool useFirst, GLuint firstTexture, GLuint secondTexture, float &scale, int &x, int &y) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        int xposw, yposw;
        glfwGetWindowPos(window, &xposw, &yposw);
        double xposm, yposm;
        glfwGetCursorPos(window, &xposm, &yposm);
        GLubyte color[4] = {255, 255, 255, 255};
        glBindTexture(GL_TEXTURE_2D, useFirst ? secondTexture : firstTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, (int)((xposm) / scale) + y,
                        (int)((WINDOW_HEIGHT - yposm) / scale) + x,
                        1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        int xposw, yposw;
        glfwGetWindowPos(window, &xposw, &yposw);
        double xposm, yposm;
        glfwGetCursorPos(window, &xposm, &yposm);
        GLubyte color[4] = {0, 0, 0, 255};
        glBindTexture(GL_TEXTURE_2D, useFirst ? secondTexture : firstTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, (int)(xposm / scale) + x,
                        SIMULATION_HEIGHT - (int)(yposm / scale) + y,
                        1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    }
}

void buttonHandler(std::chrono::time_point<std::chrono::system_clock> &lastButtonPress, bool &paused, float &fps, float &scale, int &x, int &y, bool &grid) {
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        paused = !paused;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        grid = !grid;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        fps /= 2;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        fps *= 2;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        scale *= 2;
        x += WINDOW_WIDTH / scale / 2;
        y += WINDOW_WIDTH / scale / 2;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        x -= WINDOW_WIDTH / scale / 2;
        y -= WINDOW_WIDTH / scale / 2;
        scale /= 2;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        x += 100 / scale;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        x -= 100 / scale;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        y -= 100 / scale;
        lastButtonPress = std::chrono::system_clock::now();
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS &&
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastButtonPress).count() > 200) {
        y += 100 / scale;
        lastButtonPress = std::chrono::system_clock::now();
    }

}

int main(int argc, char** argv)
{
    //Seting up glfw and glew
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Game of life", nullptr, nullptr);
    if( window == nullptr){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = static_cast<GLboolean>(true);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glDisable(GL_DEPTH_TEST);




    //Creating custom buffer
    GLuint customBuffer;
    glGenFramebuffers(1, &customBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, customBuffer);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);



    //Creating target textures
    GLuint firstTexture;
    glGenTextures(1, &firstTexture);

    std::vector<GLubyte> emptyData(SIMULATION_WIDTH * SIMULATION_HEIGHT * 4, 0);

    glBindTexture(GL_TEXTURE_2D, firstTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SIMULATION_WIDTH, SIMULATION_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);

    GLuint secondTexture;
    glGenTextures(1, &secondTexture);

    glBindTexture(GL_TEXTURE_2D, secondTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SIMULATION_WIDTH, SIMULATION_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);




    //Creating quad
    GLuint quad_VertexArrayID;
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
    };

    GLuint quad_vertexbuffer;
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);



    //Loading shaders and getting uniform locations
    GLuint golProgram = LoadShaders("golVertex.glsl","golFragment.glsl");
    GLuint renderProgram = LoadShaders("renderVertex.glsl","renderFragment.glsl");

    GLint textureID = glGetUniformLocation(golProgram, "state");
    GLint scaleID = glGetUniformLocation(golProgram, "scale");

    GLint outTextureID = glGetUniformLocation(renderProgram, "state");
    GLint outScaleID = glGetUniformLocation(renderProgram, "scale");
    GLint outPositionID = glGetUniformLocation(renderProgram, "position");
    GLint outGridID = glGetUniformLocation(renderProgram, "grid");

    bool paused = false;
    bool grid = false;

    float fps = 60;
    float scale = 1;
    int x = 0, y = 0;

    auto lastButtonPress = std::chrono::system_clock::now();
    auto lastFrame = std::chrono::system_clock::now();

    bool useFirst = true;

    do {
        glClear( GL_COLOR_BUFFER_BIT );

        if(!paused && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
                                                                            - lastFrame).count() > (1/fps*1000)) {

            lastFrame = std::chrono::system_clock::now();

            //Binding custom frame buffer and setting it's target texture
            glBindFramebuffer(GL_FRAMEBUFFER, customBuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                   useFirst ? firstTexture : secondTexture, 0);
            glViewport(0, 0, SIMULATION_WIDTH, SIMULATION_HEIGHT);


            //Setting up GOL calculation
            glUseProgram(golProgram);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, useFirst ? secondTexture : firstTexture);

            glUniform1i(textureID, 0);
            glUniform2f(scaleID, SIMULATION_WIDTH, SIMULATION_HEIGHT);


            //Calculating GOL
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDisableVertexAttribArray(0);

            //Swaping target textures
            useFirst = !useFirst;

        }

        //Setting output buffer
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(renderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, useFirst ? secondTexture : firstTexture);

        glUniform1i(outTextureID, 0);
        glUniform2f(outScaleID, scale, scale);
        glUniform2i(outPositionID, y, x);

        if (scale > 1) {
            glUniform1i(outGridID, grid);
        }
        else {
            glUniform1i(outGridID, false);
        }


        //Rendering output
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();

        mouseHandler(useFirst, firstTexture, secondTexture, scale, x, y);
        buttonHandler(lastButtonPress, paused, fps, scale, x, y, grid);



    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glfwTerminate();
}