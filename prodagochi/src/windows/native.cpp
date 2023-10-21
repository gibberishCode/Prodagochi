//
// Created by lol on 9/23/23.
//

#include "prodagochi/native.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfwpp/native.h>

namespace prodagochi {


Native::Native(glfw::Window &window) {

}

Native::~Native() = default;

void Native::probEvent() {

}

void Native::setOnKeyPressedCallback(std::function<void()> callback) {
}

std::string Native::getCurrentWindowName() {
  HWND activeWindow = GetForegroundWindow();
 if (activeWindow != NULL) {
        char windowTitle[256];
        GetWindowText(activeWindow, windowTitle, sizeof(windowTitle));
        return windowTitle;
  }
  return "";
}


std::map<unsigned long, WindowInfo>
Native::buildWindowMap(unsigned long window) {
  std::map<unsigned long, WindowInfo> windowMap;
  return windowMap;
}

std::vector<unsigned long>
Native::getAllParentsFromMap(const std::map<unsigned long, WindowInfo> &windowMap,
                             unsigned long child) {
                            return {};

}
} // namespace prodagochi
