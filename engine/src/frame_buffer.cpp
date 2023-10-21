#include "engine/frame_buffer.h"
#include "engine/texture.h"

namespace engine {
FrameBuffer::FrameBuffer(int width, int heigh) {
  glGenFramebuffers(1, &_bufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, _bufferId);

  glGenTextures(1, &_textureId);
  glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigh, 0, GL_RGB,
               GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  _texture = std::make_unique<Texture>(_textureId);

  GLuint depthrenderbuffer;
  glGenRenderbuffers(1, &depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, heigh);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depthrenderbuffer);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureId, 0);

  // Set the list of draw buffers.
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
  unBind();
}

void FrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _bufferId);
}

void FrameBuffer::unBind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture *FrameBuffer::getTexture() {
  return _texture.get();
}

} // namespace engine
