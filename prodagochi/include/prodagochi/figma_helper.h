//
// Created by lol on 9/7/23.
//

#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <memory>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "engine/ui_renderer.h"

namespace prodagochi {

std::shared_ptr<engine::Sprite> createFromSolid(const nlohmann::json &data, const glm::vec2 &pos,
                                        const glm::vec2 &size);
std::shared_ptr<engine::Sprite> createFromImage(const std::filesystem::path &path, const nlohmann::json &data,
                                        const glm::vec2 &pos,
                                        const glm::vec2 &size,
                                        std::shared_ptr<engine::Sprite> &element);

std::shared_ptr<engine::Sprite> createRect(const std::filesystem::path &path,
                                          const glm::vec2 windowSize);
std::map<std::string, std::shared_ptr<engine::UIElement>>
createUIFromFigmaFiles(const std::filesystem::path &path, glm::vec2 windowSize);

} // namespace prodagochi