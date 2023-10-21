//
// Created by lol on 9/3/23.
//

#include "engine/ui_renderer.h"
#include "engine/renderer.h"
#include <glfwpp/glfwpp.h>

namespace engine {

UIRenderer::UIRenderer(Renderer *renderer, glfw::Window *window)
    : _renederer(renderer), _root(std::make_unique<UIElement>()),
      _shader("assets/shaders/texture") {
  auto [w, h] = window->getSize();
  _projection =
      glm::ortho(0.0f, static_cast<float>(w), 0.0f, static_cast<float>(h));
}

void UIRenderer::render() {
  glDisable(GL_DEPTH_TEST);  
  _root->render(_renederer);
  glEnable(GL_DEPTH_TEST);
}

Shader UIRenderer::getShader() {
  return _shader;
}

const glm::mat4 &UIRenderer::getProjection() {
  return _projection;
}

void UIRenderer::setProjection(const glm::mat4 projection) {
  _projection = projection;
}

void UIRenderer::onClick(const glm::vec2 &pos) {
  for (const auto &child : _root->getChildren()) {
    if (child->isPointInside(pos)) {
      child->click();
    }
  }
}

} // namespace engine