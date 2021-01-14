// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "piano_app.h"

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/app/App.h>
#include <cinder/audio/audio.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <mylibrary/keysignature.h>

#include <algorithm>
#include <chrono>
#include <nlohmann/json.hpp>
#include <string>

namespace pianoapp {

using cinder::Color;
using cinder::ColorA;
using cinder::Rectf;
using cinder::app::KeyEvent;
using cinder::TextBox;
using std::string;

using std::cout;

const int letter_gap = 50;
const string white_keys_used[10] = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"};
const string black_keys_used[7] = {"2", "3", "5", "6", "7", "9", "0"};
const vector<string> possible_signatures = {"G", "D", "A", "E", "B", "F#", "C#"};

// white key audio output
cinder::audio::VoiceRef cSound;
cinder::audio::VoiceRef dSound;
cinder::audio::VoiceRef eSound;
cinder::audio::VoiceRef fSound;
cinder::audio::VoiceRef gSound;
cinder::audio::VoiceRef aSound;
cinder::audio::VoiceRef bSound;
cinder::audio::VoiceRef c1Sound;
cinder::audio::VoiceRef d1Sound;
cinder::audio::VoiceRef e1Sound;
// black key audio output
cinder::audio::VoiceRef csSound;
cinder::audio::VoiceRef dsSound;
cinder::audio::VoiceRef fsSound;
cinder::audio::VoiceRef gsSound;
cinder::audio::VoiceRef bbSound;
cinder::audio::VoiceRef cs1Sound;
cinder::audio::VoiceRef ds1Sound;

const char kNormalFont[] = "Arial";

string current_key_signature_;

nlohmann::json j;

MyApp::MyApp() {
  key_size_ = 80;
  num_white_keys_ = 10;
  board_size_ = 800;
  num_key_signatures_ = 7;
}

void MyApp::setup() {
  // create a json that contains all the key signatures
  mylibrary::KeySignature new_key = mylibrary::KeySignature();
  new_key.GenerateJson();
  std::cout << "json generated" << std::endl;
  std::ifstream i("keysignatures.json");
  // store for use later
  i >> j;
  current_key_signature_ = "G";
}

void MyApp::update() {
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();

  cinder::gl::clear();
  DrawBackground();
  DrawPiano();
  DrawKeyLetters();
  DrawInstructions();
  DrawCurrentKeySignature();
}

template <typename C>
void PrintText(const string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font(kNormalFont, 30))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - (float) box_size.x / 2, loc.y - (float) box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void MyApp::DrawPiano() const {
  // draws the white keys
  int i = 0;
  for (const string& s : white_keys_used) {
    // first draw the rectangle for each key in white
    cinder::gl::color(255, 255, 255);
    // if the button is pressed draw the key red
    for (const string& t : current_pressed_keys_) {
      if (s == t) {
        cinder::gl::color(255, 0, 0);
      }
    }
    cinder::gl::drawSolidRect(Rectf(key_size_ * i,
                                    0,
                                    key_size_ * i + key_size_,
                                    (double) board_size_ / 2));
    // then add a line to separate from the next in black
    cinder::gl::color(0, 0, 0);
    const cinder::vec2 bot = {key_size_ * i, 0};
    const cinder::vec2 top = {key_size_ * i, (double) board_size_ / 2};
    cinder::gl::drawLine(bot, top);
    i++;
  }


  // draws the black keys
  double half_black_key_size = (double) key_size_ / 3;
  int k = 0;
  for (int l = 0; l < num_white_keys_; l++) {
    // the pattern is every multiple of 7 there is not black key
    // but after every multiple of 7, a key 3 spots after is also empty
    if (!(l % 7 == 0 || (l - 3) % 7 == 0)) {
      cinder::gl::color(0, 0, 0);
      for (const string& t : current_pressed_keys_) {
        if (t == black_keys_used[k]) {
          cinder::gl::color(255, 0, 0);
        }
      }
      cinder::gl::drawSolidRect(
          Rectf((double)key_size_ * l - half_black_key_size,
                0,
                (double)key_size_ * l + half_black_key_size,
                (double)board_size_ / 4));
      k++;
    }

  }
}

void MyApp::DrawKeyLetters() const {
  // the letter size for all letters drawn
  const cinder::ivec2 size = {key_size_, 50};

  // draw on white keys
  const cinder::vec2 to_place_white = {(double) key_size_ / 2, (double) board_size_ / 2 - letter_gap};
  const Color color = Color::black();

  size_t count = 0;
  for (const string& s : white_keys_used) {
    PrintText(s, color, size, {to_place_white.x + (float) (key_size_ * count), to_place_white.y});
    count++;
  }

  // draw on black keys
  const cinder::vec2 to_place_black = {0, (double) board_size_ / 4 - letter_gap};
  const Color next_color = Color::white();

  // count starts at zero so pattern to not draw stays constant
  size_t next_count = 0;
  for (const string& t : black_keys_used) {
    // 7 and 3 are used in the pattern that a typical piano follows
    if (next_count % 7 == 0 || (next_count - 3) % 7 == 0) {
      next_count++;
    }
    PrintText(t, next_color, size,
              {to_place_black.x + (float)(key_size_ * next_count),
               to_place_black.y});
    next_count++;
  }
}

void MyApp::DrawBackground() const {
  cinder::gl::clear(Color(0, 0, 0));
}

void MyApp::DrawInstructions() const {
  string message = "Welcome to MyPianoPlayer. Each key is mapped to your"
                   " computer keyboard. Have fun!";
  const Color color = Color::white();
  const cinder::ivec2 size = {790, 100};
  PrintText(message, color, size, {board_size_ / 2, board_size_ / 2 + 100});

  string instruction = "To change visible key signature, press the SPACE bar";
  PrintText(instruction, color, size, {board_size_ / 2, board_size_ / 2 + 200});
}

void MyApp::DrawCurrentKeySignature() const {
  for (int i = 0; i < 7; i++) {
    if (current_key_signature_ == j[i]["key"]) {
      const Color color = Color::white();
      const cinder::ivec2 size = {790, 100};
      string to_print = "Key of " + current_key_signature_ + ": ";
      vector<string> to_store = j[i]["sharps"];
      for (int k = 0; k < to_store.size(); k++) {
        to_print.append(" " + to_store[k]);
      }
      PrintText(to_print, color, size, {board_size_ / 2, board_size_ / 2 + 300});
    }
  }
}

void MyApp::ChangeCurrentKeySignature() const {
  if (current_key_signature_ == possible_signatures[num_key_signatures_ - 1]) {
    current_key_signature_ = possible_signatures[0];
  } else {
    for (size_t i = 0; i < num_key_signatures_; i++) {
      if (possible_signatures[i] == current_key_signature_) {
        current_key_signature_ = possible_signatures[i + 1];
        return;
      }
    }
  }
}
bool MyApp::CheckIfAlreadyPressed(const string& name) const {
  for (const string& s : current_pressed_keys_) {
    if (s == name) {
      return true;
    }
  }
  return false;
}

void MyApp::keyDown(KeyEvent event) {
  // to exit the app
  switch (event.getCode()) {
    case KeyEvent::KEY_ESCAPE: {
      quit();
      break;
    }

    case KeyEvent::KEY_SPACE: {
      ChangeCurrentKeySignature();
      break;
    }

    // sounds for white keys
    case KeyEvent::KEY_q: {
      if (!CheckIfAlreadyPressed("Q")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("C.wav"));
        cSound = cinder::audio::Voice::create(sourceFile);
        cSound->start();
      }
      current_pressed_keys_.emplace_back("Q");
      break;
    }
    case KeyEvent::KEY_w: {
      if (!CheckIfAlreadyPressed("W")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("D.wav"));
        dSound = cinder::audio::Voice::create(sourceFile);
        dSound->start();
      }
      current_pressed_keys_.emplace_back("W");
      break;
    }
    case KeyEvent::KEY_e: {
      if (!CheckIfAlreadyPressed("E")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("E.wav"));
        eSound = cinder::audio::Voice::create(sourceFile);
        eSound->start();
      }
      current_pressed_keys_.emplace_back("E");
      break;
    }
    case KeyEvent::KEY_r: {
      if (!CheckIfAlreadyPressed("R")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("F.wav"));
        fSound = cinder::audio::Voice::create(sourceFile);
        fSound->start();
      }
      current_pressed_keys_.emplace_back("R");
      break;
    }
    case KeyEvent::KEY_t: {
      if (!CheckIfAlreadyPressed("T")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("G.wav"));
        gSound = cinder::audio::Voice::create(sourceFile);
        gSound->start();
      }
      current_pressed_keys_.emplace_back("T");
      break;
    }
    case KeyEvent::KEY_y: {
      if (!CheckIfAlreadyPressed("Y")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("A.wav"));
        aSound = cinder::audio::Voice::create(sourceFile);
        aSound->start();
      }
      current_pressed_keys_.emplace_back("Y");
      break;
    }
    case KeyEvent::KEY_u: {
      if (!CheckIfAlreadyPressed("U")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("B.wav"));
        bSound = cinder::audio::Voice::create(sourceFile);
        bSound->start();
      }
      current_pressed_keys_.emplace_back("U");
      break;
    }
    case KeyEvent::KEY_i: {
      if (!CheckIfAlreadyPressed("I")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("C1.wav"));
        c1Sound = cinder::audio::Voice::create(sourceFile);
        c1Sound->start();
      }
      current_pressed_keys_.emplace_back("I");
      break;
    }
    case KeyEvent::KEY_o: {
      if (!CheckIfAlreadyPressed("O")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("D1.wav"));
        d1Sound = cinder::audio::Voice::create(sourceFile);
        d1Sound->start();
      }
      current_pressed_keys_.emplace_back("O");
      break;
    }
    case KeyEvent::KEY_p: {
      if (!CheckIfAlreadyPressed("P")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("E1.wav"));
        e1Sound = cinder::audio::Voice::create(sourceFile);
        e1Sound->start();
      }
      current_pressed_keys_.emplace_back("P");
      break;
    }

    // sounds for black keys
    case KeyEvent::KEY_2: {
      if (!CheckIfAlreadyPressed("2")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("C_s.wav"));
        csSound = cinder::audio::Voice::create(sourceFile);
        csSound->start();
      }
      current_pressed_keys_.emplace_back("2");
      break;
    }
    case KeyEvent::KEY_3: {
      if (!CheckIfAlreadyPressed("3")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("D_s.wav"));
        dsSound = cinder::audio::Voice::create(sourceFile);
        dsSound->start();
      }
      current_pressed_keys_.emplace_back("3");
      break;
    }
    case KeyEvent::KEY_5: {
      if (!CheckIfAlreadyPressed("5")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("F_s.wav"));
        fsSound = cinder::audio::Voice::create(sourceFile);
        fsSound->start();
      }
      current_pressed_keys_.emplace_back("5");
      break;
    }
    case KeyEvent::KEY_6: {
      if (!CheckIfAlreadyPressed("6")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("G_s.wav"));
        gsSound = cinder::audio::Voice::create(sourceFile);
        gsSound->start();
      }
      current_pressed_keys_.emplace_back("6");
      break;
    }
    case KeyEvent::KEY_7: {
      if (!CheckIfAlreadyPressed("7")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("Bb.wav"));
        bbSound = cinder::audio::Voice::create(sourceFile);
        bbSound->start();
      }
      current_pressed_keys_.emplace_back("7");
      break;
    }
    case KeyEvent::KEY_9: {
      if (!CheckIfAlreadyPressed("9")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("C_s1.wav"));
        cs1Sound = cinder::audio::Voice::create(sourceFile);
        cs1Sound->start();
      }
      current_pressed_keys_.emplace_back("9");
      break;
    }
    case KeyEvent::KEY_0: {
      if (!CheckIfAlreadyPressed("0")) {
        cinder::audio::SourceFileRef sourceFile =
            cinder::audio::load(cinder::app::loadAsset("D_s1.wav"));
        ds1Sound = cinder::audio::Voice::create(sourceFile);
        ds1Sound->start();
      }
      current_pressed_keys_.emplace_back("0");
      break;
    }
  }
}
void MyApp::keyUp(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_q: {
      current_pressed_keys_.remove("Q");
      break;
    }
    case KeyEvent::KEY_w: {
      current_pressed_keys_.remove("W");
      break;
    }
    case KeyEvent::KEY_e: {
      current_pressed_keys_.remove("E");
      break;
    }
    case KeyEvent::KEY_r: {
      current_pressed_keys_.remove("R");
      break;
    }
    case KeyEvent::KEY_t: {
      current_pressed_keys_.remove("T");
      break;
    }
    case KeyEvent::KEY_y: {
      current_pressed_keys_.remove("Y");
      break;
    }
    case KeyEvent::KEY_u: {
      current_pressed_keys_.remove("U");
      break;
    }
    case KeyEvent::KEY_i: {
      current_pressed_keys_.remove("I");
      break;
    }
    case KeyEvent::KEY_o: {
      current_pressed_keys_.remove("O");
      break;
    }
    case KeyEvent::KEY_p: {
      current_pressed_keys_.remove("P");
      break;
    }

      // sounds for black keys
    case KeyEvent::KEY_2: {
      current_pressed_keys_.remove("2");
      break;
    }
    case KeyEvent::KEY_3: {
      current_pressed_keys_.remove("3");
      break;
    }
    case KeyEvent::KEY_5: {
      current_pressed_keys_.remove("5");
      break;
    }
    case KeyEvent::KEY_6: {
      current_pressed_keys_.remove("6");
      break;
    }
    case KeyEvent::KEY_7: {
      current_pressed_keys_.remove("7");
      break;
    }
    case KeyEvent::KEY_9: {
      current_pressed_keys_.remove("9");
      break;
    }
    case KeyEvent::KEY_0: {
      current_pressed_keys_.remove("0");
      break;
    }
  }
}

}  // namespace pianoapp
