#pragma once

#include <memory>

#include "common.h"

namespace engine {

class Texture;

class FrameBuffer {
public:
  FrameBuffer(int width, int heigh);
  void bind();
  void unBind();
  Texture *getTexture();

private:
  std::unique_ptr<Texture> _texture;
  GLuint _bufferId = 0;
  GLuint _textureId;
};
} // namespace engine