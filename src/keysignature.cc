//
// Created by Luke Chieng on 4/28/20.
//

#include <mylibrary/keysignature.h>

namespace mylibrary {

using std::vector;

const string notes[7] = {"A", "B", "C", "D", "E", "F", "G"};
const string sharps_order[7] = {"F#", "C#", "G#", "D#", "A#", "E#", "B#"};
const string flats_order[7] = {"Bb", "Eb", "Ab", "Db", "Gb", "Cb", "Fb"};

// the maximum amount of sharps/flats
const int max = 7;

KeySignature::KeySignature() {}

void KeySignature::GenerateJson() {
  // create json
  nlohmann::json json_file;

  // first creating the sharps portion
  int current_sharps = 3;
  for (size_t i = 0; i < max; i++) {
    // start with key name
    if (notes[i] == "F" || notes[i] == "C") {
      json_file[i]["key"] = notes[i] + "#";
    } else {
      json_file[i]["key"] = notes[i];
    }

    vector<string> sharps_contained;
    if (current_sharps > max) {
      current_sharps = current_sharps - max;
    }
    for (size_t t = 0; t < current_sharps; t++) {
      sharps_contained.push_back(sharps_order[t]);
    }
    json_file[i]["sharps"] = sharps_contained;

    // follows the pattern of adding two sharps to key signature
    // after every letter incremented

    current_sharps = current_sharps + 2;

  }
  // place into file
  std::ofstream output("keysignatures.json");
  output << json_file.dump(3);
  output.close();
}
} // namespace mylibrary