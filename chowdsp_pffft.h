/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            chowdsp_pffft
   vendor:        Chowdhury DSP
   version:       1.0.0
   name:          ChowDSP PFFFT
   description:   A wrapper of PFFFT for use as a juce::FFT instance
   dependencies:  juce_audio_basics

   website:       https://ccrma.stanford.edu/~jatin/chowdsp
   license:       BSD 3-Clause

   END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-as-null-pointer-constant",
                                     "-Wshadow-field-in-constructor",
                                     "-Wdeprecated-dynamic-exception-spec",
                                     "-Wunused-parameter",
                                     "-Wsign-conversion")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC(4100)
#if JUCE_ARM
#define PFFFT_ENABLE_NEON
#endif

#define PFFFT_ENABLE_FLOAT
#define PFFFT_ENABLE_DOUBLE
#include "pffft/pffft.h"
JUCE_END_IGNORE_WARNINGS_MSVC
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
