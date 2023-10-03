#include "engine/mesh.h"

namespace engine {
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures)
    : _vertices(std::move(vertices)), _indices(std::move(indices)),
      _textures(std::move(textures)) {
  setupMesh();
}

void Mesh::Draw(engine::Shader &shader) {
  unsigned int diffuseNr = 0;
  unsigned int specularNr = 0;
  for (unsigned int i = 0; i < _textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string glIndex;
    std::string name = "uDiffuse"; //_textures[i].type;
    if (name == "uDiffuse") {
      glIndex = std::to_string(diffuseNr++);
    } else if (name == "uS") {
      glIndex = std::to_string(specularNr++);
    }

    shader.set(name + glIndex, static_cast<int>(i));
    _textures[i].use();
    //      glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void engine::Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
               &_vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
               &_indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}
} // namespace engine