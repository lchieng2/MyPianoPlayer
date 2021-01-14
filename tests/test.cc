// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>
#include <mylibrary/keysignature.h>

using std::string;

TEST_CASE("Test created Json") {
  nlohmann::json j;
  mylibrary::KeySignature new_key = mylibrary::KeySignature();
  new_key.GenerateJson();
  std::ifstream i("keysignatures.json");
  i >> j;
  size_t possible_signatures = 7;
  SECTION("Check sharps") {
    for (size_t k = 0; k < possible_signatures; k++) {
      if (j[k]["key"] == "A") {
        std::vector<string> to_store = j[k]["sharps"];
        REQUIRE(to_store[0] == "F#");
        REQUIRE(to_store[1] == "C#");
        REQUIRE(to_store[2] == "G#");
      }

      if (j[k]["key"] == "B") {
        std::vector<string> to_store = j[k]["sharps"];
        REQUIRE(to_store[0] == "F#");
        REQUIRE(to_store[1] == "C#");
        REQUIRE(to_store[2] == "G#");
        REQUIRE(to_store[3] == "D#");
        REQUIRE(to_store[4] == "A#");
      }

      if(j[k]["key"] == "E") {
        std::vector<string> to_store = j[k]["sharps"];
        REQUIRE(to_store.size() == 4);
        REQUIRE(to_store[3] == "D#");
      }
    }
  }
}