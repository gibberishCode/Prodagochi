//
// Created by lol on 9/3/23.
//
#include <utility>

#include "engine/asset_manager.h"
#include "engine/renderer.h"
#include "engine/text_renderer.h"
#include "engine/ui_element.h"
#include "engine/ui_renderer.h"

namespace engine {

UIElement::UIElement(const glm::vec2 &pos, const glm::vec2 &size)
    : _position(pos, 0), _size(size, 1) {
  //  _anchor = glm::vec2(1, 0);
  updateLocalTransform();
}

void UIElement::render(Renderer *renderer) {
  for (auto &child : _children) {
    child->render(renderer);
  }
}

void UIElement::addChild(std::shared_ptr<UIElement> child) {
  updateChild(child);
  child->updateLocalTransform();
  _children.push_back(std::move(child));
}

void UIElement::updateChild(const std::shared_ptr<UIElement> &child) {
  std::cout << glm::to_string(_localTransform) << std::endl;
  std::cout << glm::to_string(_transform) << std::endl;
  auto nonScale = _transform;
  auto nonScale2 = _localTransform;

  nonScale[0][0] = 1;
  nonScale[1][1] = 1;
  nonScale[2][2] = 1;

  nonScale2[0][0] = 1;
  nonScale2[1][1] = 1;
  nonScale2[2][2] = 1;
  nonScale = nonScale * nonScale2;
  std::cout << glm::to_string(nonScale) << std::endl;
  std::cout << std::endl;
  child->setTranform(nonScale);
  std::for_each(child->_children.begin(), child->_children.end(),
                [this](const auto &child) { this->updateChild(child); });
}

void UIElement::setLocalTransform(const glm::mat4 &transform) {
  _localTransform = transform;
  std::for_each(_children.begin(), _children.end(),
                [this](const auto &child) { this->updateChild(child); });
}

void UIElement::setPosition(const glm::vec2 &pos) {
  _position = glm::vec3(pos, 0);
  updateLocalTransform();
  std::for_each(_children.begin(), _children.end(),
                [this](const auto &child) { this->updateChild(child); });
}

void UIElement::setTranform(const glm::mat4 &transform) {
  _transform = transform;
}

bool UIElement::isPointInside(const glm::vec2 &point) const {
  // TODO Will break if rotated
  auto transform = _transform * _localTransform;
  auto pos = glm::vec3(transform[3][0], transform[3][1], 0);
  auto scale = glm::vec3(transform[0][0], transform[1][1], 1);

  if (point.x < pos.x - scale.x || point.x > pos.x + scale.x ||
      point.y < pos.y - scale.y || point.y > pos.y + scale.y) {
    return false;
  }
  return true;
}

void UIElement::setAnchor(const glm::vec2 &anchor) {
  _anchor = anchor * -1.0f;
  updateLocalTransform();
}

void UIElement::updateLocalTransform() {
  auto correctedAnchor = _anchor;
  auto localTranslate =
      glm::translate(glm::mat4(1), glm::vec3(_size.x * correctedAnchor.x,
                                             _size.y * correctedAnchor.y, 0) *
                                       0.5f);
  auto init = glm::mat4(1);
  _localTransform = glm::translate(init, _position) * glm::scale(init, _size) *
                    glm::translate(init, glm::vec3(correctedAnchor, 0));
  std::for_each(_children.begin(), _children.end(),
                [this](const auto &child) { this->updateChild(child); });
}

void UIElement::setSize(const glm::vec2 &size) {
  _size = glm::vec3(size, 1);
  updateLocalTransform();
  std::for_each(_children.begin(), _children.end(),
                [this](const auto &child) { this->updateChild(child); });
}

Sprite::Sprite(Texture texture)
    : UIElement(glm::vec2(0), glm::vec2(0)), _texture(texture) {}

Sprite::Sprite(const glm::vec2 &pos, const glm::vec2 &size, Texture texture)
    : UIElement(pos, size), _texture(texture) {}

void Sprite::render(Renderer *renderer) {
  auto uiRenderer = renderer->getUIRenderer();
  auto shader = uiRenderer->getShader();
  auto p = uiRenderer->getProjection();
  auto m = _transform * _localTransform;
  //  std::cout << glm::to_string(_transform) << std::endl;
  //  std::cout << glm::to_string(m) << std::endl;
  //  std::cout << glm::to_string(_localTransform) << std::endl << std::endl;
  auto mp = p * m;
  shader.use();
  shader.set("uOpacity", _opacity);
  shader.set("uColor", _color);
  renderer->renderQuad(_texture, shader, mp);
  UIElement::render(renderer);
}

void Sprite::setColor(const glm::vec3 &color) {
  if (color.x > 1) {
    _color = color / 255.0f;
    return;
  }
  _color = color;
}

Sprite::Sprite(const glm::vec2 &pos, const glm::vec2 &size,
               const std::string &texturePath)
    : Sprite(pos, size, AssetManager::getAsset<Texture>(texturePath)) {}

void Sprite::setOpacity(float opacity) {
  _opacity = opacity;
}

Button::Button(const glm::vec2 &pos, const glm::vec2 &size)
    : UIElement(pos, size) {
  addChild(std::make_shared<Sprite>(
      glm::vec2(0, 0), size,
      AssetManager::getAsset<Texture>("assets/textures/rect.png")));
}

void Button::setOnClickCallback(std::function<void()> callback) {
  _onClickedCallback = std::move(callback);
}

void Button::click() {
  if (_onClickedCallback) {
    _onClickedCallback();
  }
}

ProgressBar::ProgressBar(const glm::vec2 &pos, const glm::vec2 &size)
    : UIElement(pos, size) {
  _bar = std::make_shared<Sprite>(
      glm::vec2(0, 0), size,
      AssetManager::getAsset<Texture>("assets/textures/rect.png"));
  addChild(_bar);
  _bar->setAnchor(glm::vec2(-1, 0));
  _maxValue = size.x;
}

void ProgressBar::setValue(float value) {
  _bar->setSize(glm::vec2(_maxValue * value, _size.y));
  _value = value;
}

InternalText::InternalText(const glm::vec2 &pos, std::string text,
                           float textWidth)
    : UIElement(pos, glm::vec2(1, 1)), _text(std::move(text)),
      _textWidth(textWidth) {}

void InternalText::render(engine::Renderer *renderer) {
  auto textRenderer = renderer->getTextRenderer();
  auto uiRenderer = renderer->getUIRenderer();
  auto p = uiRenderer->getProjection();
  auto m = _transform * _localTransform;
  auto mp = p * m;
  textRenderer->renderText(mp, _text.c_str(), _textWidth);
}

Text::Text(const glm::vec2 &pos, const glm::vec2 &size, std::string text)
    : UIElement(pos, size * 0.5f) {
  addChild(
      std::make_shared<InternalText>(glm::vec2(0, 0), std::move(text), size.x));
}

Text::Text(const glm::vec2 &pos, std::string text)
    : UIElement(pos, glm::vec2(1, 1)) {
  auto textSize = TextRenderer::getInstance()->getTextSize(text);
  _size = glm::vec3(textSize * 0.5f, 1);
  updateLocalTransform();
  addChild(std::make_shared<InternalText>(glm::vec2(0, 0), std::move(text),
                                          text.size() * 48));
}

void Text::render(engine::Renderer *renderer) {
  UIElement::render(renderer);
}
} // namespace engine
