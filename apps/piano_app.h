// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <cinder/audio/Voice.h>
#include <mylibrary/keysignature.h>

namespace pianoapp {

using std::string;
using std::vector;
using std::list;

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void keyUp(cinder::app::KeyEvent) override;

 private:
  void DrawPiano() const;
  void DrawKeyLetters() const;
  void DrawBackground() const;
  void DrawInstructions() const;
  void DrawCurrentKeySignature() const;
  void ChangeCurrentKeySignature() const;
  bool CheckIfAlreadyPressed(const string& name) const;

 private:
  size_t key_size_;
  size_t num_white_keys_;
  size_t board_size_;
  size_t num_key_signatures_;
  list<string> current_pressed_keys_;
};

}  // namespace pianoapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
