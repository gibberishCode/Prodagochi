//
// Created by lol on 9/3/23.
//
#include "engine/shader.h"

namespace engine {

GLuint Shader::_currentProgram = {};

Shader::Shader(const std::string &shaderPath)
    : Shader(shaderPath + ".vertex", shaderPath + ".fragment") {}

Shader::Shader(std::string_view vertexShaderPath,
               std::string_view fragnmentShaderPath) {
  _program = loadShaders(vertexShaderPath.data(), fragnmentShaderPath.data());
}

void Shader::set(std::string_view name, const glm::vec4 &data) {
  GLint location = getUniformLocation(name);
  glUniform4f(location, data.x, data.y, data.z, data.w);
}

void Shader::set(std::string_view name, const glm::vec3 &data) {
  GLint location = getUniformLocation(name);
  glUniform3f(location, data.x, data.y, data.z);
}

void Shader::set(std::string_view name, const glm::vec2 &data) {
  GLint location = getUniformLocation(name);
  glUniform2f(location, data.x, data.y);
}

void Shader::set(std::string_view name, const glm::mat4 &data) {
  GLint location = getUniformLocation(name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
}

void Shader::set(std::string_view name, float data) {
  GLint location = getUniformLocation(name);
  glUniform1f(location, data);
}

void Shader::set(std::string_view name, int data) {
  GLint location = getUniformLocation(name);
  glUniform1i(location, data);
}

GLint Shader::getUniformLocation(std::string_view name) const {
  assert(_currentProgram == _program);
  // TODO check dict performance
  GLint location = glGetUniformLocation(_program, name.data());
  if (location < 0) {
    std::cerr << name << " not found" << std::endl;
  }
  return location;
}

void Shader::use() const {
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glUseProgram(_program);
  _currentProgram = _program;
}

void Shader::unUse() const {
  glUseProgram(0);
  _currentProgram = 0;
}

} // namespace engine