//
// Created by lol on 9/2/23.
//

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <thread>

#include "engine/asset_manager.h"
#include "engine/model.h"
#include "engine/renderer.h"
#include "engine/text_renderer.h"
#include "engine/ui_element.h"
#include "engine/ui_renderer.h"
#include "glm/fwd.hpp"
#include "prodagochi/app_description.h"
#include "prodagochi/figma_helper.h"
#include "prodagochi/native.h"
#include "prodagochi/prodgagochi.h"
#include "prodagochi/productivity_state.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "rest_server/httplib.h"

using Window = glfw::Window;
using Clock = std::chrono::system_clock;
// using Renderer = engine::Renderer;
// using Shader = engine::Shader;
// using Texture = engine::Texture;
// using Sprite = engine::Sprite;
using namespace engine;
using namespace httplib;
using json = nlohmann::json;

// TODO update glViewport

namespace prodagochi {

void to_json(json& j, const AppInfo& p) {
        j = json{{"title", p.Title}, {"type", p.Type}};
  }

void from_json(const json& j, AppInfo& p) {
    j.at("title").get_to(p.Title);
    j.at("type").get_to(p.Type);
}

Prodgagochi::Prodgagochi(int width, int height) {
  _glfw.reset(new glfw::GlfwLibrary(glfw::init()));
  glfw::WindowHints{.decorated = false,
                    // .floating = true,
                    .transparentFramebuffer = false,
                    .contextVersionMajor = 4,
                    .contextVersionMinor = 5,
                    .openglProfile = glfw::OpenGlProfile::Core}
      .apply();
  _window = std::make_unique<Window>(width, height, "Prodagochi");
  glfw::makeContextCurrent(*(_window.get()));
  glfw::swapInterval(0);
  _native = std::make_unique<Native>(*_window);
}

Prodgagochi::~Prodgagochi() = default;

void Prodgagochi::onTabChanged(std::string tabUrl) {
  std::cout << tabUrl << std::endl;
}


void Prodgagochi::init() {
  _renderer = std::make_unique<Renderer>(_window.get());
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
  _avatarShader3D = Shader("assets/shaders/model");
  _avatarTexture =
      AssetManager::getAsset<Texture>("assets/textures/texture.png");
   _model = new Model("assets/models/monkey.fbx");
  createUI();
      //auto l = new Model("assets/models//*monkey.*/fbx");
  auto [w, h] = _window->getSize();
   _frameBuffer = new FrameBuffer(w, h);
  //  _serverThread = {serv, "Server"};
  _serverThread= std::thread{&Prodgagochi::listenForTabSwitch, this};
  loadSettings();

 
  //  _window->focusEvent.setCallback([this](glfw::Window &, bool state) {
  //    if (!state) {
  //      this->setState(std::make_unique<ClosedState>());
  //    } else {~
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

glm::mat4 _mvp;
float dt = 0;

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


    auto projection = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
   auto view = glm::translate(glm::mat4(1), glm::vec3(0, 1, -3));
   auto pos = glm::vec3{0, 0, 2};
   auto target = glm::vec3{0, 0, 0};
   glm::mat4 viewMatrix = glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));
   auto model = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1));
   glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f),
                                          glm::vec3(1.0f, 0.0f, 0.0f));
   dt += 0.001f;
   glm::mat4 rotationMatrix1 = glm::rotate(glm::mat4(1.0f), glm::radians(dt),
                                          glm::vec3(0.0f, 0.0f, 1.0f));
   model *= rotationMatrix * rotationMatrix1;
   _mvp = projection * viewMatrix * model;

}

void Prodgagochi::render() {
  //   _frameBuffer->bind();
  //  glClear(GL_COLOR_BUFFER_BIT |
  //          GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
  //  glEnable(GL_DEPTH_TEST);
  _avatarShader3D.use();
   _avatarShader3D.set("uMvp", _mvp);
   _model->Draw(_avatarShader3D);
  //  _frameBuffer->unBind();
  // _renderer->getUIRenderer()->render();
  // renderAvatar();
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
  //_avatarShader.set("uUvScale", 1);
  // std::cout << _frameBuffer->getTexture()->getId() << std::endl;
  _renderer->renderQuad(*(_frameBuffer->getTexture()), _avatarShader, mp);
}

void Prodgagochi::renderAvatarState() {

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


void Prodgagochi::listenForTabSwitch() {
  Server svr;

  svr.Post("/",
  [&](const Request &req, Response &res, const ContentReader &content_reader) {
      std::string body;
      content_reader([&](const char *data, size_t data_length) {
        body.append(data, data_length);
        return true;
      });
      json json = json.parse(body); 
      if (json.contains("url")) {
        onTabChanged(std::move(json["url"]));
      }
  });
  svr.listen("127.0.0.1", 3000);
}

void Prodgagochi::renderBackground() {}

void Prodgagochi::loadSettings() {
  if (!std::filesystem::exists(SETTINGS_FILENAME)) {
    return;
  }

  std::ifstream stream(SETTINGS_FILENAME);
  nlohmann::json j;
  stream >> j;
  _apps = j["apps"].template get<std::vector<AppInfo>>();
}

} // namespace prodagochi