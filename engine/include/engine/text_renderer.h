//
// Created by lol on 9/3/23.
//

#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "common.h"
#include "shader.h"

namespace engine {

class Renderer;

struct Character {
  glm::ivec2 Size;      // Size of glyph
  glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
  unsigned int Advance; // Offset to advance to next glyph
  double U;
};

struct Atlas {
  float Width{};
  float Height{};
  GLuint Texture{};
};

class TextRenderer {
public:
  TextRenderer(Renderer *renderer);
  ~TextRenderer() = default;
  glm::vec2 getTextSize(std::string_view text, float textArea = 0);
  void renderText(const glm::mat4 &mvp, const char *text,
                  float textAreaWidth = 0);
  // TODO Either prorperly use singletons with some service locator or avoid
  // this This is needed because the only way for text object to know its size
  // is through glyphs
  static TextRenderer *getInstance();

private:
  void initLetters();

private:
  static TextRenderer *_instanse;
  const int FontSize = 48;
  const int LoadLetters = 128 - 32;
  Atlas _atlas{};
  Shader _textShader;
  std::unordered_map<FT_ULong, Character> Characters;
};
} // namespace engine