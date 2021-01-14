//
// Created by Luke Chieng on 4/28/20.
//

#ifndef FINALPROJECT_MYLIBRARY_KEYSIGNATURE_H_
#define FINALPROJECT_MYLIBRARY_KEYSIGNATURE_H_

#include <string>
#include <nlohmann/json.hpp>
#include <cinder/app/App.h>

namespace mylibrary {

using std::string;

class KeySignature {
 public:
  KeySignature();
  void GenerateJson();
};
}

#endif  // FINALPROJECT_MYLIBRARY_KEYSIGNATURE_H_
