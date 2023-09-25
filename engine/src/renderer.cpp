//
// Created by lol on 9/3/23.
//
#include <glfwpp/glfwpp.h>

#include "engine/renderer.h"
#include "engine/text_renderer.h"
#include "engine/ui_renderer.h"

using Window = glfw::Window;

namespace engine {

Renderer::Renderer(Window *window) {
  if (!gladLoadGLLoader((GLADloadproc)glfw::getProcAddress)) {
    throw std::runtime_error("Failed to load gl with glad");
  }
  auto [x, y] = window->getSize();
  glViewport(0, 0, x, y);
  glEnable(GL_DEBUG_OUTPUT);
  //  glDebugMessageCallback(MessageCallback, 0);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  createQuad();
  _textRenderer = std::make_unique<TextRenderer>(this);
  _uiRenderer = std::make_unique<UIRenderer>(this, window);
}

Renderer::~Renderer() = default;

void Renderer::createQuad() {
  GLuint vbo;
  glGenVertexArrays(1, &_quadVao);
  glBindVertexArray(_quadVao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, DATA_PER_VERTEX * 6 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  int stride = DATA_PER_VERTEX * sizeof(GLfloat);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                        (void *)(3 * sizeof(GLfloat)));
}

void Renderer::renderQuad(Texture texture, Shader shader,
                          const glm::mat4 &transform) {
  texture.use();
  renderQuad(shader, transform, QuadVertices);
}

void Renderer::renderQuad(Shader shader, const glm::mat4 &transform) {
  renderQuad(shader, transform, QuadVertices);
}

void Renderer::renderQuad(Shader shader, const glm::mat4 &transform,
                          std::span<float, 6 * 5> vertices) {

  // TODO dont switch when neccery
  // Batch draw
  shader.use();
  shader.set("uMvp", transform);
  glBindVertexArray(_quadVao);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size_bytes(), vertices.data());
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

TextRenderer *Renderer::getTextRenderer() const {
  return _textRenderer.get();
}

UIRenderer *Renderer::getUIRenderer() const {
  return _uiRenderer.get();
}

void Renderer::clear() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render(Texture teture, Shader shader, const glm::mat4 &transform,
                      float *data, int size) {
  throw;
}

} // namespace engine