//
// Created by lol on 9/7/23.
//

#pragma once

#include "prodagochi/figma_helper.h"

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace engine;

namespace prodagochi {

std::shared_ptr<Sprite> createFromSolid(const json &data, const glm::vec2 &pos,
                                        const glm::vec2 &size) {
  auto jsonColor = data["fills"][0]["color"];
  glm::vec3 color{jsonColor["r"], jsonColor["g"], jsonColor["b"]};
  float opacity = data["opacity"];
  auto rect = std::make_shared<Sprite>(pos, size, "assets/textures/rect.png");
  rect->setAnchor({-1, 1});
  rect->setColor(color);
  rect->setOpacity(opacity);
  return rect;
}

std::shared_ptr<Sprite> createFromImage(const fs::path &path, const json &data,
                                        const glm::vec2 &pos,
                                        const glm::vec2 &size,
                                        std::shared_ptr<Sprite> &element) {

  auto image = data["fills"][0]["src"];
  auto imagePath = path.parent_path().parent_path() / image;
  element = std::make_shared<Sprite>(pos, size, imagePath.string());
  return element;
}

std::shared_ptr<Sprite> createRect(const fs::path &path,
                                          const glm::vec2 windowSize) {
  // return std::shared_ptr<Sprite>(nullptr);
  std::ifstream f(path);
  json data = json::parse(f);
  std::string type = data["fills"][0]["type"];
  glm::vec2 pos{data["x"], data["y"]};
  pos.y = windowSize.y - pos.y;
  glm::vec2 size{data["width"], data["height"]};
  size *= 0.5f;
  std::shared_ptr<Sprite> element;
  if (type == "SOLID") {
    element = createFromSolid(data, pos, size);
  } else if (type == "IMAGE") {
    element = createFromImage(path, data, pos, size, element);
  }
  element->setAnchor({-1, 1});
  return element;
}

std::map<std::string, std::shared_ptr<UIElement>>
createUIFromFigmaFiles(const fs::path &path, glm::vec2 windowSize) {
  std::map<std::string, std::shared_ptr<UIElement>> ret;
  if (fs::is_directory(path)) {
    auto jsonPath = path / "json";
    for (const auto &entry : fs::directory_iterator(jsonPath)) {
      if (fs::is_regular_file(entry.path())) {
         auto rect = createRect(entry.path(), windowSize);
         
         ret.insert({entry.path().stem().string(), rect});
      }
      //  else {
      //   std::cerr << "The specified path is not a directory." << std::endl;
      // }
    }
  }
  return ret;
}

} // namespace prodagochi