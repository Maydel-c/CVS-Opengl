#include <GLFW/glfw3.h>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int WIDTH = 1920;
int HEIGHT = 1013;

const char *TITLE = "CVS: Definitive Edition";
GLFWwindow *window = nullptr;

class Shader {
private:
  unsigned int mId;

  std::string LoadFile(const std::string &path) {
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      file.open(path);
      std::stringstream stream;
      stream << file.rdbuf();
      file.close();
      code = stream.str();
    } catch (const std::ifstream::failure &e) {
      std::cerr << "ERROR: Loading file | PATH: " << path
                << " | REASON: " << e.what() << std::endl;
    }
    return code;
  }

  unsigned int CompileShader(const std::string &srcCode, bool isVert) {
    const char *code = srcCode.c_str();
    int success;
    char infoLog[512];
    unsigned int shader = isVert ? glCreateShader(GL_VERTEX_SHADER)
                                 : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cerr << "ERROR: " << infoLog << std::endl;
    }
    return shader;
  }

  void CreateShaderProgram(unsigned int &vert, unsigned int &frag) {
    mId = glCreateProgram();
    glAttachShader(mId, vert);
    glAttachShader(mId, frag);
    glLinkProgram(mId);

    int success;
    char infoLog[512];
    glGetProgramiv(mId, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(mId, 512, NULL, infoLog);
      std::cerr << "ERROR: Linking shader program: " << infoLog << std::endl;
    }
  }

public:
  Shader(const std::string &vertPath, const std::string &fragPath) {
    std::string vCode = LoadFile(vertPath);
    std::string fCode = LoadFile(fragPath);
    unsigned int vert = CompileShader(vCode, true);
    unsigned int frag = CompileShader(fCode, false);

    CreateShaderProgram(vert, frag);
  }

  ~Shader() { glDeleteProgram(mId); }

  void Use() { glUseProgram(mId); }

  template <typename T> void SetValue(const std::string &name, const T &val) {
    unsigned int loc = glGetUniformLocation(mId, name.c_str());

    if constexpr (std::is_same_v<T, int>)
      glUniform1i(loc, val);
    else if constexpr (std::is_same_v<T, bool>)
      glUniform1i(loc, (int)val);
    else if constexpr (std::is_same_v<T, float>)
      glUniform1f(loc, val);
    else if constexpr (std::is_same_v<T, glm::vec2>)
      glUniform2fv(loc, 1, glm::value_ptr(val));
    else if constexpr (std::is_same_v<T, glm::vec3>)
      glUniformMatrix3fv(loc, 1, glm::value_ptr(val));
    else if constexpr (std::is_same_v<T, glm::mat4>)
      glUniformMatrix4fv(loc, 1, GL_FLOAT, glm::value_ptr(val));
  }
}
