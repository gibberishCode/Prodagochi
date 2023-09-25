//
// Created by lol on 9/2/23.
//

#include "prodagochi/prodgagochi.h"

void createUI() {
  Texture texture{"path"};
  auto sprite = std::make_shared<Sprite>(0, 0, 0, 0, texture);
  auto button = std::make_shared<Button>(0, 0, 0, 0, "some_text", texture);
  button->setOnPressedCallback([](){});
}

class Avatar{
  void draw() {
    //use avatar shader
    //set uniforms
    //draw vertices
  }
};

void render() {
  //set projection
  //set uniforms
  drawVertices(vertices);
}


int main() {
//  init();
//  while(true)
  //detect current window
  //calulate productivity
  //if needs to be notified
  // avatar popup or system pop

  //open state
  //draw background
  //draw avatar
  //draw avatar state

  //if trying to close
  //prompt minimze of exit

//  void drawAvatar() {
//    RendererContext(Shader,)
//  }
  

}