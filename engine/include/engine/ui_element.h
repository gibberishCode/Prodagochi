//
// Created by lol on 9/3/23.
//

#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "common.h"
#include "texture.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

namespace engine {

class Renderer;

class UIElement {
public:
  UIElement() = default;
  UIElement(const glm::vec2 &pos, const glm::vec2 &size);
  virtual ~UIElement() = default;
  virtual void render(Renderer *renderer);

  virtual void click() {}

  bool isPointInside(const glm::vec2 &point) const;
  void setPosition(const glm::vec2 &pos);
  void setSize(const glm::vec2 &size);
  void addChild(std::shared_ptr<UIElement> child);
  void setTranform(const glm::mat4 &transform);
  void setAnchor(const glm::vec2 &anchor);
  void setLocalTransform(const glm::mat4 &transform);

  const glm::mat4 &getLocalTransform() const {
    return _localTransform;
  }

  const std::vector<std::shared_ptr<UIElement>> &getChildren() const {
    return _children;
  }

  void updateLocalTransform();

private:
  void updateChild(const std::shared_ptr<UIElement> &child);

protected:
  glm::vec2 _anchor{0};
  glm::vec3 _position{};
  glm::vec3 _size{1};
  glm::mat4 _localTransform{1};
  glm::mat4 _transform{1};

private:
  std::vector<std::shared_ptr<UIElement>> _children;
};

class Sprite : public UIElement {
public:
  Sprite(Texture texture);
  Sprite(const glm::vec2 &pos, const glm::vec2 &size,
         const std::string &texturePath);
  Sprite(const glm::vec2 &pos, const glm::vec2 &size, Texture texture);
  void render(Renderer *renderer) override;
  void setColor(const glm::vec3 &color);
  void setOpacity(float opacity);

private:
  Texture _texture;
  glm::vec3 _color{1, 1, 1};
  float _opacity{1};
};

class Button : public UIElement {
public:
  Button(const glm::vec2 &pos, const glm::vec2 &size);
  void click() override;
  void setOnClickCallback(std::function<void()> callback);

private:
  std::function<void()> _onClickedCallback;
};

class Text : public UIElement {
public:
  Text(const glm::vec2 &pos, const glm::vec2 &size, std::string text);
  Text(const glm::vec2 &pos, std::string text);

  void render(engine::Renderer *renderer) override;
};

class InternalText : public UIElement {
public:
  InternalText(const glm::vec2 &pos, std::string text, float textWidth);

  void render(engine::Renderer *renderer) override;

private:
  std::string _text;
  float _textWidth;
};

class ProgressBar : public UIElement {
public:
  ProgressBar(const glm::vec2 &pos, const glm::vec2 &size);
  void setValue(float value);

  float getValue() const {
    return _value;
  }

private:
  float _value{};
  float _maxValue{};
  std::shared_ptr<Sprite> _bar;
};

} // namespace engine