#pragma once
#include <vector>

#include "common.h"
#include "shader.h"
#include "texture.h"

namespace engine {

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  void Draw(Shader &shader);

private:
  void setupMesh();

private:
  std::vector<Vertex> _vertices;
  std::vector<unsigned int> _indices;
  std::vector<Texture> _textures;
  unsigned int VAO, VBO, EBO;
};
} // namespace engine