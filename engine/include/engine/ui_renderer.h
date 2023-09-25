//
// Created by lol on 9/3/23.
//

#pragma once

#include <memory>
#include <vector>

#include "common.h"
#include "engine/shader.h"
#include "texture.h"
#include "ui_element.h"

namespace glfw {
class Window;
}

namespace engine {

class Renderer;

class UIRenderer {
public:
  UIRenderer(Renderer *renderer, glfw::Window *window);
  ~UIRenderer() = default;
  void render();
  Shader getShader();
  void onClick(const glm::vec2 &pos);

  UIElement *getRoot() {
    return _root.get();
  }

  void setProjection(const glm::mat4 projection);
  const glm::mat4 &getProjection();

private:
  Renderer *_renederer;
  std::unique_ptr<UIElement> _root;
  Shader _shader;
  glm::mat4 _projection;
};

} // namespace engine