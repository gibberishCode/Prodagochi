//
// Created by lol on 9/23/23.
//

#include "prodagochi/native.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xmu/WinUtil.h> // `apt-get install libxmu-dev`
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h>

#undef None
#define GLFW_EXPOSE_NATIVE_X11
#include <glfwpp/native.h>

struct Wrapper {

  Display *X11Display;
};

namespace prodagochi {

Display *getDisplay() {
  Display *display = glfw::native::getX11Display();
  return display;
}

Window getFocusedWindow(Display *display) {
  Window focusedWindow;
  int revertTo;
  XGetInputFocus(display, &focusedWindow, &revertTo);
  return focusedWindow;
}

unsigned char *get_string_property(Display *display, Window window,
                                   char *property_name, unsigned char *prop) {
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;

  filter_atom = XInternAtom(display, property_name, True);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1000, False,
                              AnyPropertyType, &actual_type, &actual_format,
                              &nitems, &bytes_after, &prop);
  //  check_status(status, window);
  return prop;
}

// unsigned long get_long_property(char *property_name) {
//   get_string_property(property_name);
//   unsigned long long_property =
//       prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
//   return long_property;
// }

std::string getWindowName(Display *display, Window window) {
  Atom wm_class = XInternAtom(display, "WM_CLASS", False);

  Atom actual_type;
  int actual_format;
  unsigned long nitems, bytes_after;
  unsigned char *prop_data = nullptr;

  if (XGetWindowProperty(display, window, wm_class, 0, 1024, False, XA_STRING,
                         &actual_type, &actual_format, &nitems, &bytes_after,
                         &prop_data) == Success &&
      prop_data) {
    if (actual_type == XA_STRING && actual_format == 8) {
      std::string appClass(reinterpret_cast<char *>(prop_data));
      return appClass;
      // std::cout << "class way: " << appClass << std::endl;
    }
    XFree(prop_data);
  }
  return "";
}

Native::Native(glfw::Window &window) {
  // _display = getDisplay();
  _wrapper = new Wrapper();
  _wrapper->X11Display = getDisplay();
  auto display = _wrapper->X11Display;
  display = XOpenDisplay(nullptr);
  if (display == nullptr) {
    std::cerr << "Unable to open X display." << std::endl;
  }

  Window root = DefaultRootWindow(display);
  auto nativeWindow = glfw::native::getX11Window(window);
  XSelectInput(display, nativeWindow, KeyPressMask | KeyReleaseMask);
  _windowsMap = buildWindowMap(root);
  //  for (const auto &entry : windowMap) {
  //    Window window = entry.first;
  //    WindowInfo info = entry.second;
  //    std::cout << "Window ID: " << window << std::endl;
  //    std::cout << "Parent ID: " << info.parent << std::endl;
  //    std::cout << "Children IDs:";
  //    for (const auto &child : info.children) {
  //      std::cout << " " << child;
  //    }
  //    std::cout << std::endl;
  //  }
}

Native::~Native() = default;

void Native::probEvent() {
  auto display = _wrapper->X11Display;
  if (display == nullptr) {
    std::cerr << "Unable to open X display." << std::endl;
    return;
  }
  while (XPending(display) > 0) {
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == KeyPress) {
      if (_onKeyPressedCallback) {
        _onKeyPressedCallback();
      }
      KeyCode keycode = event.xkey.keycode;
      std::cout << "Key pressed: " << (int)keycode << std::endl;
    } else if (event.type == KeyRelease) {
      KeyCode keycode = event.xkey.keycode;
      std::cout << "Key released: " << (int)keycode << std::endl;
    }
  }
}

void Native::setOnKeyPressedCallback(std::function<void()> callback) {
  _onKeyPressedCallback = std::move(callback);
}

std::string Native::getCurrentWindowName() {
  auto display = _wrapper->X11Display;
  auto window = getFocusedWindow(display);
  unsigned char *prop;
  //  printf("_NET_WM_NAME: %s\n",
  //         get_string_property(display, window, "_NET_WM_NAME", prop));
  //  printf("_NET_WM_NAME: %s\n", get_string_property("_NET_WM_NAME"));
  auto windows = getAllParentsFromMap(_windowsMap, window);
  std::string test{};
  for (auto w : windows) {
    test += " " + getWindowName(display, w);
  }
  std::cout << test << std::endl;
  return getWindowName(display, window);
}

//  printf("_NET_WM_NAME: %s\n",
//         get_string_property(display, window, "_NET_WM_NAME", prop));
//  printf("_NET_WM_NAME: %s\n", get_string_property("_NET_WM_NAME"));

std::map<unsigned long, WindowInfo>
Native::buildWindowMap(unsigned long window) {
  auto display = _wrapper->X11Display;
  std::map<Window, WindowInfo> windowMap;

  Window root, parent, *children;
  unsigned int num_children;

  if (XQueryTree(display, window, &root, &parent, &children, &num_children)) {
    WindowInfo info;
    info.parent = parent;

    for (unsigned int i = 0; i < num_children; ++i) {
      info.children.push_back(children[i]);
      auto childMap = buildWindowMap(children[i]);
      windowMap.insert(childMap.begin(), childMap.end());
    }

    windowMap[window] = info;

    XFree(children);
  }

  return windowMap;
}

std::vector<Window>
Native::getAllParentsFromMap(const std::map<Window, WindowInfo> &windowMap,
                             Window child) {
  std::vector<Window> parents;

  while (child) {
    parents.push_back(child);

    // Use the window map to get the parent from the previously built map
    auto it = windowMap.find(child);
    if (it != windowMap.end()) {
      child = it->second.parent;
    } else {
      break;
    }
  }

  return parents;
}
} // namespace prodagochi
