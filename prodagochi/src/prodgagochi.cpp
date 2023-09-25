//
// Created by lol on 9/2/23.
//

#include <chrono>
#include <memory>

#include "engine/asset_manager.h"
#include "engine/renderer.h"
#include "engine/text_renderer.h"
#include "engine/ui_renderer.h"
#include "prodagochi/app_description.h"
#include "prodagochi/figma_helper.h"
#include "prodagochi/native.h"
#include "prodagochi/prodgagochi.h"
#include "prodagochi/productivity_state.h"

using Window = glfw::Window;
using Clock = std::chrono::system_clock;
// using Renderer = engine::Renderer;
// using Shader = engine::Shader;
// using Texture = engine::Texture;
// using Sprite = engine::Sprite;
using namespace engine;

// TODO update glViewport

namespace prodagochi {

Prodgagochi::Prodgagochi(int width, int height) {
  _glfw.reset(new glfw::GlfwLibrary(glfw::init()));
  glfw::WindowHints{.decorated = false,
                    .transparentFramebuffer = false,
                    .contextVersionMajor = 4,
                    // .floating = true,
                    .contextVersionMinor = 5,
                    .openglProfile = glfw::OpenGlProfile::Core}
      .apply();
  _window = std::make_unique<Window>(width, height, "Prodagochi");
  glfw::makeContextCurrent(*(_window.get()));
  glfw::swapInterval(0);
  _renderer = std::make_unique<Renderer>(_window.get());
  _native = std::make_unique<Native>(*_window);
  //  createUI();
}

Prodgagochi::~Prodgagochi() = default;

void Prodgagochi::init() {
  _window->sizeEvent.setCallback([this](auto &window, float w, float h) {
    glViewport(0, 0, w, h);
    this->_renderer->getUIRenderer()->setProjection(
        glm::ortho(0.0f, w, 0.0f, h));
  });
  _window->mouseButtonEvent.setCallback(
      [this](auto &window, auto button, auto state, auto bit) {
        auto [x, y] = _window->getCursorPos();
        _renderer->getUIRenderer()->onClick(glm::vec2(x, y));
      });
  _avatarShader = Shader("assets/shaders/texture");
  //  _avatarTexture = Texture("assets/textures/texture.png");
  _avatarTexture =
      AssetManager::getAsset<Texture>("assets/textures/texture.png");
  createUI();
  //  _window->focusEvent.setCallback([this](glfw::Window &, bool state) {
  //    if (!state) {
  //      this->setState(std::make_unique<ClosedState>());
  //    } else {
  //      // this->setState(std::make_unique<OpenState>());
  //    }
  //  });
  //
  //  _window->iconifyEvent.setCallback([this](glfw::Window &, bool state) {
  //    if (state) {
  //      this->setState(std::make_unique<ClosedState>());
  //    } else {
  //      // this->setState(std::make_unique<OpenState>());
  //    }
  //  });
  //  auto f = _window->getAttribFloating();
  //  std::cout << "float" << f << std::endl;
  //  // _renderer.init(&(_window.value()));
  //  _eventManager.subscribe(this, &App::onAngryCallback);
  //  _shader = Shader("assets/shaders/texture.vertex",
  //                   "assets/shaders/texture.fragment");
  //  _textShader =
  //      Shader("assets/shaders/text.vertex", "assets/shaders/text.fragment");
  //  _texture = Texture("assets/textures/texture.png");
  //
}

void Prodgagochi::update() {
  std::string name = _native->getCurrentWindowName();
  //  std::cout << name << std::endl;
  //  auto [w, h] = _window->getSize();
  //  auto center = glm::vec2(w, h) / 2.0f;
  //  static float size = 0;
  //  size += 0.000005f;
  //  _spriteTest->setValue(size);
  //  _spriteTest->setSize(glm::vec2(size, 30));
  //  _spriteTest->setPosition(center);
}

void Prodgagochi::render() {
  _renderer->getUIRenderer()->render();
  renderAvatar();
}

void Prodgagochi::run() {
  auto prevTime = Clock::now();
  int frames{};
  int fps{};
  float time{};
  auto fpsCountre = [&frames, &time, &fps](float dt) {
    frames++;
    time += dt;
    if (time >= 1) {
      time = 0;
      fps = frames;
      frames = 0;
      std::cout << "FPS: " << fps << std::endl;
    }
  };
  while (!_window->shouldClose()) {
    _renderer->clear();
    auto currentTime = Clock::now();
    std::chrono::duration<float> duration = currentTime - prevTime;
    prevTime = currentTime;
    fpsCountre(duration.count());
    update();
    render();
    _window->swapBuffers();
    glfw::pollEvents();
  }
}

void Prodgagochi::renderAvatar() {
  auto [x, y] = _window->getSize();
  glm::mat4 p = glm::ortho<float>(0.0f, x, 0.0f, y);
  auto translate =
      glm::translate(glm::mat4(1), glm::vec3(82 + 90, y - 64 - 90, 0));
  auto scale = glm::scale(glm::mat4(1), glm::vec3(90, 90, 1));
  auto m = translate * scale;
  auto mp = p * m;
  _avatarShader.use();
  _avatarShader.set("uUvScale", 0.25f);
  _renderer->renderQuad(_avatarTexture, _avatarShader, mp);
}

void Prodgagochi::createUI() {
  auto uiRenderer = _renderer->getUIRenderer();
  auto root = uiRenderer->getRoot();
  auto [w, h] = _window->getSize();
  auto center = glm::vec2(w, h) / 2.0f;

  auto background = std::make_shared<Sprite>(
      center, glm::vec2(w, h),
      AssetManager::getAsset<Texture>("assets/textures/rect.png"));
  background->setColor(glm::vec3(34, 34, 34));
  root->addChild(background);

  auto ui = createUIFromFigmaFiles("assets/figma_json", {w, h});
  for (const auto &[name, element] : ui) {
    root->addChild(element);
  }
}

void Prodgagochi::renderBackground() {}

} // namespace prodagochi