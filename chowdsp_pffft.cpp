#include "chowdsp_pffft.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#undef JUCE_DSP_H_INCLUDED // HACK!
#include <juce_dsp/juce_dsp.cpp> // NOLINT

// reference: https://github.com/soundradix/JUCE/blob/ad6afe48faecc02e21a774d1bf643cb30852cd4b/modules/juce_dsp/frequency/juce_FFT.cpp#L946

namespace juce::dsp
{

class PrettyFastFFT : public FFT::Instance
{
public:
    static constexpr auto priority = 7;

    static PrettyFastFFT* create (const int order)
    {
        if (order < 5)
        {
            // Not supported according to PFFFT's docs:
            //
            // > supports only transforms for inputs of length N of the form
            // > N=(2^a)*(3^b)*(5^c), a >= 5, b >=0, c >= 0
            jassertfalse;
            return nullptr;
        }
        return new PrettyFastFFT (order);
    }

    void perform (const Complex<float>* input, Complex<float>* output, bool inverse) const noexcept override
    {
        pffft_transform_ordered (setupComp, (const float*) input, (float*) output, workBuf, inverse ? PFFFT_BACKWARD : PFFFT_FORWARD);
        if (inverse)
            FloatVectorOperations::multiply ((float*) output, 1.0f / float (1 << order), 1 << (order + 1));
    }

    void performRealOnlyForwardTransform (float* inoutData, bool ignoreNegativeFreqs) const noexcept override
    {
        pffft_transform_ordered (setupReal, inoutData, inoutData, workBuf, PFFFT_FORWARD);
        const int nyquist = 1 << (order - 1);
        inoutData[2 * nyquist] = inoutData[1];
        inoutData[2 * nyquist + 1] = 0.0f;
        inoutData[1] = 0.0f;
        if (! ignoreNegativeFreqs)
        {
            // Silly anti-feature to produce redundant negative freqs!
            auto out = (Complex<float>*) inoutData;
            for (int i = 1; i < nyquist; ++i)
                out[nyquist + i] = std::conj (out[nyquist - i]);
        }
    }

    void performRealOnlyInverseTransform (float* inoutData) const noexcept override
    {
        JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4334)
        inoutData[1] = inoutData[1 << order];
        JUCE_END_IGNORE_WARNINGS_MSVC
        pffft_transform_ordered (setupReal, inoutData, inoutData, workBuf, PFFFT_BACKWARD);
        FloatVectorOperations::multiply (inoutData, 1.0f / float (1 << order), 1 << order);
    }

    ~PrettyFastFFT() override
    {
        pffft_destroy_setup (setupReal);
        pffft_destroy_setup (setupComp);
        pffft_aligned_free (workBuf);
    }

private:
    explicit PrettyFastFFT (int order_) : order (order_)
    {
        setupReal = pffft_new_setup (1 << order, PFFFT_REAL);
        jassert (setupReal != nullptr);

        setupComp = pffft_new_setup (1 << order, PFFFT_COMPLEX);
        jassert (setupComp != nullptr);

        workBuf = (float*) pffft_aligned_malloc (sizeof (float) << (order + 1));
        jassert (workBuf != nullptr);
    }

    PFFFT_Setup* setupReal;
    PFFFT_Setup* setupComp;
    float* workBuf;
    int order;
};

FFT::EngineImpl<PrettyFastFFT> prettyFastFFT;

} // namespace juce::dsp

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-as-null-pointer-constant",
                                     "-Wshadow",
                                     "-Wmissing-prototypes",
                                     "-Wsign-conversion",
                                     "-Wcast-align",
                                     "-Wfloat-equal",
                                     "-Wvla-extension")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4005)
#include "pffft/pffft.c" // NOLINT
#include "pffft/pffft_common.c" // NOLINT
JUCE_END_IGNORE_WARNINGS_MSVC
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
