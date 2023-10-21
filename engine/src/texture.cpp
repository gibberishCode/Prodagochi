//
// Created by lol on 9/3/23.
//
#define STB_IMAGE_IMPLEMENTATION
#include "engine/texture.h"

namespace engine {

Texture::Texture(std::string_view path) {
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &_textureId);
  glBindTexture(GL_TEXTURE_2D, _textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, channels;
  unsigned char *data = stbi_load(path.data(), &width, &height, &channels, 0);
  GLenum format;
  if (data) {
    if (channels == 1) {
      format = GL_RED;
    } else if (channels == 3) {
      format = GL_RGB;
    } else if (channels == 4) {
      format = GL_RGBA;
    } else {
      std::cerr << "unsuported image " << channels << std::endl;
      // Handle unsupported format
      stbi_image_free(data);
      return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    // TODO do something clever
    std::cerr << "Failed to load texture " << path << std::endl;
  }
  //  glBindTexture(GL_TEXTURE_2D, _textureId);
  stbi_image_free(data);
}

Texture::Texture(GLuint id) : _textureId(id) {}

void Texture::use() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureId);
}
} // namespace engine