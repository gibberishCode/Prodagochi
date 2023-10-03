//
// Created by lol on 9/3/23.
//

#pragma once

#include "string_view"

#include "common.h"
#include "load_shader.h"

namespace engine {

class Shader {
public:
  Shader() = default;
  Shader(const std::string &shaderPath);
  Shader(std::string_view vertexShaderPath,
         std::string_view fragnmentShaderPath);
  void set(std::string_view name, const glm::mat4 &data);
  void set(std::string_view name, float data);
  void set(std::string_view name, int data);
  void set(std::string_view name, const glm::vec2 &data);
  void set(std::string_view name, const glm::vec3 &data);
  void set(std::string_view name, const glm::vec4 &data);
  void use() const;
  void unUse() const;

private:
  GLint getUniformLocation(std::string_view name) const;

private:
  static GLuint _currentProgram;
  GLuint _program{};
};
} // namespace engine