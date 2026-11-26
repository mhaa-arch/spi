#include <spi/spi.hpp>
#include "../examples/fake.hpp"

#include <cassert>

using namespace mhaa::spi;
using namespace example;

class PackedController final : public ByteExchange
{
public:
    Status configure(const Config &config) override
    {
        if (config.word_bits != 8 && config.word_bits != 24)
        {
            return Status::Unsupported;
        }
        word_bits_ = config.word_bits;
        return Status::Success;
    }

    Result exchange(const Word *tx, Word *rx, size_t count, uint32_t) override
    {
        return exchange_one(tx, rx, count);
    }

    Result exchange_bytes(const uint8_t *tx, uint8_t *rx, size_t count, uint32_t) override
    {
        if (word_bits_ && word_bits_ != 8)
        {
            return {Status::Unsupported, 0};
        }
        return exchange_one(tx, rx, count);
    }

private:
    template <typename Value>
    Result exchange_one(const Value *tx, Value *rx, size_t count)
    {
        if (!word_bits_)
        {
            return {Status::Busy, 0};
        }
        if (!count)
        {
            return {Status::Success, 0};
        }
        rx[0] = static_cast<Value>(tx[0] ^ 0xFF);
        return {count == 1 ? Status::Success : Status::Timeout, 1};
    }

    uint8_t word_bits_ = 0;
};

int main()
{
    Fake controller;
    SPI spi(controller);
    Word buffer[] = {0x11, 0x22, 0x33};
    assert(spi.exchange(nullptr, buffer, 3, 5).status == Status::InvalidArgument);
    assert(spi.exchange(nullptr, nullptr, 0, 0).status == Status::InvalidArgument);
    assert(spi.exchange(nullptr, nullptr, 0, 5).status == Status::Success && controller.calls == 0);
    controller.limit = 2;
    const Result partial = spi.exchange(buffer, buffer, 3, 5);
    assert(partial.status == Status::Timeout && partial.completed == 2);
    assert(buffer[0] == 0x800011 && buffer[1] == 0x800022 && buffer[2] == 0x33);
    controller.limit = 0;
    Word received = 7;
    assert(spi.exchange_word(0x44, received, 5) == Status::Timeout && received == 7);
    controller.limit = 1;
    assert(spi.exchange_word(0x44, received, 5) == Status::Success && received == 0x800044);

    assert(spi.configure({Mode::Mode0, BitOrder::MostSignificantFirst, Role::Controller, 0, 24}) == Status::InvalidArgument);
    assert(spi.configure({Mode::Mode0, BitOrder::MostSignificantFirst, Role::Target, 50, 24}) == Status::InvalidArgument);
    assert(spi.configure({Mode::Mode0, BitOrder::MostSignificantFirst, Role::Controller, 1, 8}) == Status::Unsupported);
    assert(spi.configure({Mode::Mode0, BitOrder::MostSignificantFirst, Role::Controller, 1, 24}) == Status::Success);

    assert(spi.configure({Mode::Mode0, BitOrder::MostSignificantFirst, Role::Target, 0, 24}) == Status::Success);

    PackedController packed;
    SPI byte_bus(packed);
    uint8_t bytes[] = {0x11, 0x33};
    assert(spi.exchange_bytes(bytes, bytes, 2, 5).status == Status::Unsupported);
    Config config{Mode::Mode3, BitOrder::MostSignificantFirst, Role::Controller, 1000000, 24};
    assert(byte_bus.configure(config) == Status::Success);
    assert(byte_bus.exchange_bytes(bytes, bytes, 2, 5).status == Status::Unsupported);
    config.word_bits = 8;
    assert(byte_bus.configure(config) == Status::Success);
    assert(byte_bus.exchange_bytes(nullptr, bytes, 2, 10).status == Status::InvalidArgument);
    const Result byte_result = byte_bus.exchange_bytes(bytes, bytes, 2, 10);
    assert(byte_result.status == Status::Timeout && byte_result.completed == 1);
    assert(bytes[0] == 0xEE && bytes[1] == 0x33);
    assert(byte_bus.exchange_word(0x05, received, 10) == Status::Success && received == 0xFA);
}
