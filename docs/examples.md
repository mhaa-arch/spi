# Run the SPI Core example

The Core borrows the fake controller, configures 24-bit words in mode 0 at 1 MHz and exchanges `0x24` within 5 ms. The example checks the fake's received word `0x800024`. The behavior test includes another implementation offering packed bytes and verifies preservation of the uncompleted suffix.

## Example program

The [program](../examples/example.cpp) uses the [local fake](../examples/fake.hpp) to demonstrate injection. A concrete Adapter implementing the same Contract can be supplied to the same facade.

```cpp
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
```

## Build and run

Build from this repository root. Set `contract_include` to the matching Contract's `include/` directory. The value below applies to the MHAA development workspace. Both programs exit with status 0 on success and print no output. On Windows execute `example.exe` and `behavior.exe`.

```sh
contract_include=../../contracts/spi-contract/include
g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -fno-rtti -fno-exceptions -Iinclude -I"$contract_include" examples/example.cpp -o example
./example
g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -fno-rtti -fno-exceptions -Iinclude -I"$contract_include" tests/behavior.cpp -o behavior
./behavior
```

See the [behavior test](../tests/behavior.cpp) for the additional cases described above.
