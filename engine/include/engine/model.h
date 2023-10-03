#pragma once

#include <string>
#include <string_view>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"
#include "texture.h"

namespace engine {
class Model {
public:
  Model(std::string_view path);
  void Draw(Shader &shader);

private:
  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName);

private:
  std::vector<Mesh> _meshes;
  std::string _directory;
};
} // namespace engine