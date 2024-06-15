// Test09.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef GLAD_IMPLEMENTATION
#define GLAD_IMPLEMENTATION
#include <glad/glad.h>
#endif // GLAD_IMPLEMENTATION

// GLFW_INCLUDE_NONE information
// https://www.glfw.org/docs/latest/build_guide.html
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif // GLFW_INCLUDE_NONE

// ������ѧ����
#include <glm/glm.hpp>
// ��δʹ��
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <windows.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>

// ���ڳߴ�
const int WIDTH = 600;
const int HEIGHT = 600;

// ��ɫ������
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";
// ���������ɫ
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // ��ɫ
"}\0";

// ������ɫ������
unsigned int CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
  {
    // ����������ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ��鶥����ɫ���������
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ����Ƭ����ɫ��
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ���Ƭ����ɫ���������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ������ɫ������
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �����ɫ���������Ӵ���
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
  }
}

// �� glfw ���ڱ�����ʾ����
void SetWindowTitleUTF8(GLFWwindow* window, const char* title)
{
  int wlen = MultiByteToWideChar(CP_ACP, 0, title, -1, NULL, 0);
  wchar_t* wstr = new wchar_t[wlen];
  MultiByteToWideChar(CP_ACP, 0, title, -1, wstr, wlen);

  int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
  char* utf8str = new char[utf8len];
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8str, utf8len, NULL, NULL);

  glfwSetWindowTitle(window, utf8str);

  delete[] wstr;
  delete[] utf8str;
}

// ��ʼ�� GLFW �� OpenGL
bool InitOpenGL()
{
  // ��ʼ�� GLFW
  if (!glfwInit())
  {
    return false;
  }

  // ���� GLFW �汾
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // ��������
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return false;
  }
  SetWindowTitleUTF8(window, "OpenGL ��ת��������");

  // ���õ�ǰ����Ϊ�����
  glfwMakeContextCurrent(window);

  // ��ʼ�� GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  // �����ӿ�
  glViewport(0, 0, WIDTH, HEIGHT);

  // ������Ȳ���
  glEnable(GL_DEPTH_TEST);

  return true;
}

// �������������
void DrawCube(unsigned int shaderProgram, unsigned int VAO)
{
  glBindVertexArray(VAO);

  glDrawArrays(GL_LINE_LOOP, 0, 4); // ǰ��
  glDrawArrays(GL_LINE_LOOP, 4, 4); // ����
  glDrawArrays(GL_LINE_LOOP, 8, 4); // ���
  glDrawArrays(GL_LINE_LOOP, 12, 4); // �Ҳ�
  glDrawArrays(GL_LINE_LOOP, 16, 4); // ����
  glDrawArrays(GL_LINE_LOOP, 20, 4); // �ײ�

  glBindVertexArray(0);
}

int main()
{
  // ��ʼ�� OpenGL
  if (!InitOpenGL())
  {
    return 1;
  }

  // ������ɫ������
  unsigned int shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

  // �����嶥��
  float vertices[] = {
    // ǰ��
    -0.65f, -0.65f,  0.65f,
     0.65f, -0.65f,  0.65f,
     0.65f,  0.65f,  0.65f,
    -0.65f,  0.65f,  0.65f,

    // ����
    -0.65f, -0.65f, -0.65f,
     0.65f, -0.65f, -0.65f,
     0.65f,  0.65f, -0.65f,
    -0.65f,  0.65f, -0.65f,

    // ���
    -0.65f, -0.65f,  0.65f,
    -0.65f, -0.65f, -0.65f,
    -0.65f,  0.65f, -0.65f,
    -0.65f,  0.65f,  0.65f,

    // �Ҳ�
     0.65f, -0.65f,  0.65f,
     0.65f, -0.65f, -0.65f,
     0.65f,  0.65f, -0.65f,
     0.65f,  0.65f,  0.65f,

     // ����
     -0.65f,  0.65f,  0.65f,
      0.65f,  0.65f,  0.65f,
      0.65f,  0.65f, -0.65f,
     -0.65f,  0.65f, -0.65f,

     // �ײ�
     -0.65f, -0.65f,  0.65f,
      0.65f, -0.65f,  0.65f,
      0.65f, -0.65f, -0.65f,
     -0.65f, -0.65f, -0.65f,
  };

  // ���� VAO �� VBO
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // �� VAO �� VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // ���������ݸ��Ƶ� VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // ���ö�������ָ��
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // �����
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // ģ�͡���ͼ��ͶӰ����
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

  // ��ת�Ƕȱ���
  float angleX = 0;
  float angleY = 0;
  float angleZ = 0;

  // ��ȡ����̨���ھ��
  HWND hConsole = GetConsoleWindow();
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  // ֡�ʿ���
  double timePrevious = glfwGetTime();
  double timeLast = 0.0;
  int fps = 0;
  double standard_frame_rate = 60; // Ŀ��֡��
  double frameTimeLimit = 1.0 / standard_frame_rate;  // ��ʼ��֡ʱ��
  float fltFactor = 0.0f;
  double timeFramePrevious = glfwGetTime();
  double timeFrameLast = 0.0;

  // ���ÿ���� (ò��û����)
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_MULTISAMPLE);
  // �����߿�
  glLineWidth(1);

  // ��ѭ��
  while (!glfwWindowShouldClose(glfwGetCurrentContext()))
  {
    // ֡�ʿ����߼�
    timeFramePrevious = glfwGetTime();
    if ((timeFramePrevious - timeFrameLast) >= 1.0f)
    {
      // �������
      COORD pos = { 1, 1 };
      DWORD charsWritten;
      std::string fpsStr = "��ǰ֡��: " + std::to_string(fps) + "   ";
      WriteConsoleOutputCharacterA(hStdOut, fpsStr.c_str(), fpsStr.length(), pos, &charsWritten);
      pos = { 1, 2 };
      std::string frameTimeStr = "��֡ʱ��: " + std::to_string(frameTimeLimit) + "   ";
      WriteConsoleOutputCharacterA(hStdOut, frameTimeStr.c_str(), frameTimeStr.length(), pos, &charsWritten);

      timeFrameLast = timeFramePrevious;

      // ���ݵ�ǰ֡�ʵ�����֡ʱ�䣬��֡������ standard_frame_rate
      if (fps > standard_frame_rate)
      {
        if (fps > (standard_frame_rate * 2))
        {
          fltFactor = 1;
        }
        else if (fps > (standard_frame_rate * 1.5))
        {
          fltFactor = 0.5;
        }
        else if (fps > (standard_frame_rate * 1.1))
        {
          fltFactor = 0.1;
        }
        else
        {
          fltFactor = 0.01;
        }
        frameTimeLimit = frameTimeLimit + (frameTimeLimit * fltFactor);
      }
      else if (fps < standard_frame_rate)
      {
        if (fps < (standard_frame_rate / 1.1))
        {
          fltFactor = 0.1;
        }
        else if (fps < (standard_frame_rate / 1.5))
        {
          fltFactor = 0.5;
        }
        else if (fps < (standard_frame_rate / 2))
        {
          fltFactor = 1;
        }
        frameTimeLimit = frameTimeLimit - (frameTimeLimit * fltFactor);
      }
      fps = 0;

      if (frameTimeLimit < 0.000001)
      {
        frameTimeLimit = 0.1;
      }
    }

    timePrevious = timeFramePrevious;
    if ((timePrevious - timeLast) < frameTimeLimit)
    {
      continue;
    }
    timeLast = timePrevious;
    fps += 1;

    // �����ɫ������
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ʹ����ɫ������
    glUseProgram(shaderProgram);

    // ÿһ֡��ת�Ƕȣ�Ҳ���ٶȿ���
    angleX += (5.7 * 0.00002);
    angleY += (2.1 * 0.00002);
    angleZ += (9.33 * 0.00002);
    // ������ת����
    model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    // ����ģ�͡���ͼ��ͶӰ����
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // ����������
    DrawCube(shaderProgram, VAO);

    // ����������
    glfwSwapBuffers(glfwGetCurrentContext());
    // �����¼�
    glfwPollEvents();
  }

  // ɾ�� VAO, VBO ����ɫ������
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // �˳� GLFW
  glfwTerminate();

  return 0;
}