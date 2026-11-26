#include <spi/spi.hpp>
#include "fake.hpp"

using namespace mhaa::spi;
using namespace example;

int main()
{
    Fake controller;
    SPI spi(controller);
    const Config config{Mode::Mode0, BitOrder::MostSignificantFirst, Role::Controller, 1000000, 24};
    if (spi.configure(config) != Status::Success)
    {
        return 1;
    }
    Word word = 0;
    return spi.exchange_word(0x24, word, 5) == Status::Success && word == 0x800024 ? 0 : 1;
}
