//
// Created by lol on 9/2/23.
//

#pragma once

#include <memory>

#include "engine/renderer.h"
#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/ui_element.h"
#include <glfwpp/glfwpp.h>

namespace prodagochi {
class Native;

class Prodgagochi {
public:
  Prodgagochi(int width, int height);
  ~Prodgagochi();
  void init();
  void run();

private:
  void update();
  void render();
  void createUI();
  void renderAvatar();
  void renderBackground();
  void renderState();

private:
  std::unique_ptr<glfw::GlfwLibrary> _glfw;
  std::unique_ptr<glfw::Window> _window;
  std::unique_ptr<glfw::Window> _popUpWindow;
  std::unique_ptr<engine::Renderer> _renderer;
  std::unique_ptr<Native> _native;
  engine::Shader _avatarShader;
  engine::Texture _avatarTexture;
  std::shared_ptr<engine::ProgressBar> _spriteTest;
  class UIRenderer *_uiRenderer;
};

} // namespace prodagochi