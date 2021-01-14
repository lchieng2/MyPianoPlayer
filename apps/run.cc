// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include "piano_app.h"

using cinder::app::App;
using cinder::app::RendererGl;


namespace pianoapp {

const int kSamples = 8;
const int kWidth = 800;
const int kHeight = 800;

void SetUp(App::Settings* settings) {
  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("My Piano Player");
}
}  // namespace myapp


// This is a macro that runs the application.
CINDER_APP(pianoapp::MyApp,
           RendererGl(RendererGl::Options().msaa(pianoapp::kSamples)),
           pianoapp::SetUp)
