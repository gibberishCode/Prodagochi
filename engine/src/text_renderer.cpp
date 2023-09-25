//
// Created by lol on 9/3/23.
//
#include "engine/text_renderer.h"
#include "engine/renderer.h"
#include <algorithm>

namespace engine {

TextRenderer *TextRenderer::_instanse = nullptr;

TextRenderer::TextRenderer(Renderer *renderer) {
  initLetters();
  _textShader = Shader("assets/shaders/text");
  _instanse = this;
}

void TextRenderer::initLetters() {
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cerr << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
  }

  FT_Face face;
  if (FT_New_Face(ft, "assets/fonts/Noto.ttf", 0, &face)) {
    std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
  }

  FT_Set_Pixel_Sizes(face, 0, FontSize);
  glPixelStorei(GL_UNPACK_ALIGNMENT,
                1); // disable byte-alignment restriction

  _atlas.Width = LoadLetters * FontSize;
  _atlas.Height = FontSize;

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &_atlas.Texture);
  glBindTexture(GL_TEXTURE_2D, _atlas.Texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _atlas.Width, _atlas.Height, 0, GL_RED,
               GL_UNSIGNED_BYTE, 0);
  float x{};
  float y{};
  // for (unsigned char c = 0; c < 128; c++) {
  for (FT_ULong c = 32; c < 128; c++) {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    auto glyph = face->glyph;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width,
                    glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE,
                    glyph->bitmap.buffer);
    Character character = {
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<uint32_t>(face->glyph->advance.x), x / _atlas.Width};

    x += character.Size.x;
    Characters.insert(std::pair<FT_ULong, Character>(c, character));
  }
}

void TextRenderer::renderText(const glm::mat4 &mvp, const char *text,
                              float textAreaWidth) {
  struct point {
    GLfloat x;
    GLfloat y;
    GLfloat z = 0;
    GLfloat u;
    GLfloat v;
  } coords[6 * strlen(text)];

  float x{};
  float y{};
  int n{};
  float scale = 1;
  for (const char *p = text; *p; p++) {
    Character ch = Characters[*p];
    if (x > textAreaWidth) {
      x = 0;
      y -= _atlas.Height;
    }
    float x2 = x + ch.Bearing.x * scale;
    float y2 = y - (-ch.Size.y / _atlas.Height - ch.Bearing.y) * scale;

    float left = ch.U;    //  c[*p].left;
    float bw = ch.Size.x; // c[*p].bw;
    float bh = ch.Size.y; // c[*p].bh;
    float w = ch.Size.x;  // c[*p].bw * sx;
    float h = ch.Size.y;  // c[*p].bh * sy;
    x += (ch.Advance >> 6);
    float right = left + bw / _atlas.Width;
    float bottom = bh / _atlas.Height;

    coords[n++] = {x2, y2, 0, left, 0};
    coords[n++] = {x2 + w, y2, 0, right, 0};
    coords[n++] = {x2, y2 - h, 0, left, bottom};
    coords[n++] = {x2 + w, y2, 0, right, 0};
    coords[n++] = {x2, y2 - h, 0, left, bottom};
    coords[n++] = {x2 + w, y2 - h, 0, right, bottom};
  }
  auto offset = glm::vec3{y <= -_atlas.Height ? textAreaWidth : x, y, 0};
  auto mOffset = glm::translate(glm::mat4(1), -offset / 2.0f);
  _textShader.use();
  glBindTexture(GL_TEXTURE_2D, _atlas.Texture);
  _textShader.set("uMvp", mvp * mOffset);
  glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, n);
}

glm::vec2 TextRenderer::getTextSize(std::string_view text, float textArea) {
  float x{};
  float y{};
  for (auto c : text) {
    Character ch = Characters[c];
    if (textArea != 0 && x > textArea) {
      x = 0;
      y += _atlas.Height;
    }
    x += (ch.Advance >> 6);
  }
  return {x, y};
}

TextRenderer *TextRenderer::getInstance() {
  return _instanse;
}
} // namespace engine

// auto shift2 = glm::vec3(xpos, ypos + h / 2, 0);
// auto sc = glm::scale(glm::mat4(1.0f), glm::vec3(w, h, 1));
// auto tr = glm::translate(
//     glm::mat4(1.0f), glm::vec3(xpos + shift.x + w / 2, ypos + h / 2, 1));

// glm::mat4 letterTransform = tr * sc;
