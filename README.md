# chowdsp_pffft

Combining a few things:
- JUCE's FFT API
- Julien Pommier's [PFFFT library](https://bitbucket.org/jpommier/pffft/src/master/)
- SoundRadix's [JUCE-style wrapper](https://github.com/soundradix/JUCE/blob/ad6afe48faecc02e21a774d1bf643cb30852cd4b/modules/juce_dsp/frequency/juce_FFT.cpp#L946) for PFFFT

I'm not 100% sure what the licensing rules are around this code.
PFFFT has a BSD-like license, and the SoundRadix code seems to
be under the same licensing rules as JUCE. So if you're abiding
by the JUCE license, I think you should be fine to use this?

In order to avoid symbols clashing, you must make sure that the
`juce_dsp` module is not being compiled alongside this one. If
you are in control of all the modules being used, this should be
pretty easy to do, but if you're using third-party modules that
require `juce_dsp` then I'm not sure what you should do. Since
this module will compile the `juce_dsp` code for you, you may want
to manually define `JUCE_MODULE_AVAILABLE_juce_dsp=1` so that
other modules in your system are aware.
