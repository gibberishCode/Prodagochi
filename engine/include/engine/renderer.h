//
// Created by lol on 9/3/23.
//
#pragma once

#include <memory>
#include <span>

#include "common.h"
#include "shader.h"
#include "texture.h"

// #include "ui_render.h"

namespace glfw {
class Window;
}

// clang-format off
static float QuadVertices[] = {-1, -1, 0, 0, 1,
                               1, -1, 0, 1, 1,
                               1, 1, 0, 1, 0,
                               1, 1, 0, 1, 0,
                               -1, 1, 0, 0, 0,
                               -1, -1, 0, 0, 1};

// clang-format on
namespace engine {
class TextRenderer;
class UIRenderer;

// class UIRenderer;

class Renderer {

public:
  Renderer(glfw::Window *window);
  ~Renderer();
  void render(Texture teture, Shader shader, const glm::mat4 &transform,
              float *data, int size);
  void renderQuad(Texture texture, Shader shader, const glm::mat4 &transform);
  void renderQuad(Shader shader, const glm::mat4 &transform);
  void clear();

  void renderQuad(Shader shader, const glm::mat4 &transform,
                  std::span<float, 6 * 5> vertices);

  TextRenderer *getTextRenderer() const;
  UIRenderer *getUIRenderer() const;

private:
  void createQuad();

private:
  const int DATA_PER_VERTEX{5}; // For simplicity x,y,z,u,v
  GLuint _quadVao{};
  std::unique_ptr<TextRenderer> _textRenderer;
  std::unique_ptr<UIRenderer> _uiRenderer;
};
} // namespace engine