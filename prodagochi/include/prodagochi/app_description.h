
#pragma once
#include <stdexcept>
#include <string>

namespace prodagochi {

enum class AppType {
  VeryProductive,
  Productive,
  Neutral,
  Distractive,
  VeryDistractive
};

constexpr const char *appTypeTostring(AppType e) throw() {
  switch (e) {
  case AppType::VeryProductive:
    return "VeryProductive";
  case AppType::Productive:
    return "Productive";
  case AppType::Neutral:
    return "Neutral";
  case AppType::Distractive:
    return "Distractive";
  case AppType::VeryDistractive:
    return "VeryDistractive";
  default:
    throw std::invalid_argument("Unimplemented item");
  }
}

struct AppInfo {
  std::string Title;
  AppType Type;
};

} // namespace prodagochi