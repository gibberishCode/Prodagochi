//
// Created by lol on 9/4/23.
//

#pragma once
#include "engine/shader.h"
#include "engine/texture.h"
#include <any>
#include <filesystem>
#include <string_view>
#include <unordered_map>

namespace fs = std::filesystem;

namespace engine {

template <typename T>
concept StringConstructible = requires(std::string str) {
  { T(str) } -> std::same_as<T>; // Requires a constructor from std::string
};

class AssetManager {
public:
  template <StringConstructible Asset>
  static Asset getAsset(const std::string &path) {
    auto it = _pathToAsset.find(path);
    if (it != _pathToAsset.end()) {
      return std::any_cast<Asset>((it->second));
    }
    if (!fs::exists(path)) {
      throw std::runtime_error("Path does not exist: " + path);
    }
    auto [where, _] = _pathToAsset.emplace(path, Asset(path));
    return std::any_cast<Asset>(where->second);
  }

private:
  static std::unordered_map<std::string, std::any> _pathToAsset;
};

} // namespace engine
