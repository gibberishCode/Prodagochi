#pragma once

#include <functional>
#include <glfwpp/window.h>
#include <iostream>
#include <map>
#include <string>
#include <utility>
class Wrapper;

namespace prodagochi {

struct WindowInfo {
  unsigned long parent;
  std::vector<unsigned long> children;
};

class Native {
public:
  ~Native();
  Native(glfw::Window &window);
  void probEvent();
  void setOnKeyPressedCallback(std::function<void()> callback);
  std::string getCurrentWindowName();

private:
  std::map<unsigned long, WindowInfo> buildWindowMap(unsigned long window);
  std::vector<unsigned long>
  getAllParentsFromMap(const std::map<unsigned long, WindowInfo> &windowMap,
                       unsigned long child);

private:
  std::function<void()> _onKeyPressedCallback;
  Wrapper *_wrapper;
  std::map<unsigned long, WindowInfo> _windowsMap;
};

} // namespace prodagochi