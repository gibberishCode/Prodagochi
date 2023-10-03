//
// Created by lol on 9/3/23.
//

#pragma once

// #define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <string_view>

#include "common.h"
#include "stbi_image.h"

namespace engine {

class Texture {
public:
  Texture() = default;
  explicit Texture(std::string_view path);
  explicit Texture(GLuint id);

  void use() const;

  GLuint getId() {
    return _textureId;
  }

private:
  GLuint _textureId{};
};
} // namespace engine