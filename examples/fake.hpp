#ifndef MHAA_SPI_EXAMPLE_FAKE_HPP
#define MHAA_SPI_EXAMPLE_FAKE_HPP

#include <spi/controller.hpp>

namespace example
{
    using namespace mhaa::spi;

    class Fake final : public Controller
    {
    public:
        Status configure(const Config &config) override
        {
            return config.word_bits == 24 ? Status::Success : Status::Unsupported;
        }
        Result exchange(const Word *tx, Word *rx, size_t count, uint32_t) override
        {
            ++calls;
            const size_t completed = count < limit ? count : limit;
            for (size_t i = 0; i < completed; ++i)
            {
                rx[i] = tx[i] ^ UINT32_C(0x800000);
            }
            return {completed == count ? Status::Success : Status::Timeout, completed};
        }

        size_t limit = 3;
        unsigned calls = 0;
    };
}

#endif
