//
// Created by lol on 9/23/23.
//

#pragma once
#include <algorithm>

class ProductivityState {
public:
  void setProductivity(float productivity) {
    _productivity = productivity;
    _productivity = std::clamp(_productivity, 0.0f, 100.0f);
  }

  float getProductivity() const {
    return _productivity;
  }

private:
  float _productivity{};
  float _focus{};
  float _commitment{};
};
