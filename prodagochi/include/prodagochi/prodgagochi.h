#pragma once

#include <memory>
#include <string>
#include <thread>

#include <glfwpp/glfwpp.h>

#include "engine/frame_buffer.h"
#include "engine/renderer.h"
#include "engine/model.h"
#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/ui_element.h"
#include "app_description.h"
#include "prodagochi/app_description.h"

namespace prodagochi {
class Native;

class Prodgagochi {
public:
  Prodgagochi(int width, int height);
  ~Prodgagochi();
  void init();
  void run();

private:
  void loadSettings();
  void update();
  void render();
  void createUI();
  void renderAvatar();
  void renderBackground();
  void renderAvatarState();
  void onTabChanged(std::string tabName);
  void listenForTabSwitch();

private:
  std::unique_ptr<glfw::GlfwLibrary> _glfw;
  std::unique_ptr<glfw::Window> _window;
  std::unique_ptr<glfw::Window> _popUpWindow;
  std::unique_ptr<engine::Renderer> _renderer;
  std::unique_ptr<Native> _native;
  std::vector<AppInfo> _apps;
  std::thread _serverThread;
  engine::Shader _avatarShader;
  engine::Shader _avatarShader3D;
  engine::Texture _avatarTexture;
  engine::FrameBuffer *_frameBuffer;
  std::shared_ptr<engine::ProgressBar> _spriteTest;
  AppSettings _settings;
  engine::Model *_model;
  class UIRenderer *_uiRenderer;

  const std::string SETTINGS_FILENAME{"settings.json"};
};

} // namespace prodagochi