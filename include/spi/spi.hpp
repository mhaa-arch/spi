#ifndef MHAA_SPI_HPP
#define MHAA_SPI_HPP

#include <stdint.h>
#include <stddef.h>
#include <spi/byte_exchange.hpp>
#include <spi/controller.hpp>
#include <spi/types.hpp>

namespace mhaa
{
    namespace spi
    {
        /** @brief Exchanges words on one borrowed SPI peripheral. */
        class SPI
        {
        public:
            explicit SPI(Controller &controller) : controller_(controller), bytes_(nullptr) {}
            explicit SPI(ByteExchange &controller) : controller_(controller), bytes_(&controller) {}

            Status configure(const Config &config)
            {
                const bool valid_rate = config.role == Role::Controller
                                            ? config.frequency_hz != 0
                                            : config.frequency_hz == 0;
                if (!valid_rate || config.word_bits == 0 || config.word_bits > 32)
                {
                    return Status::InvalidArgument;
                }
                return controller_.configure(config);
            }

            Result exchange(const Word *tx, Word *rx, size_t count, uint32_t timeout_ms)
            {
                if (!timeout_ms || (count && (!tx || !rx)))
                {
                    return {Status::InvalidArgument, 0};
                }
                if (!count)
                {
                    return {Status::Success, 0};
                }
                return controller_.exchange(tx, rx, count, timeout_ms);
            }

            /** @brief Exchanges packed bytes in eight-bit mode under the same transaction rules. */
            Result exchange_bytes(const uint8_t *tx, uint8_t *rx, size_t count, uint32_t timeout_ms)
            {
                if (!timeout_ms || (count && (!tx || !rx)))
                {
                    return {Status::InvalidArgument, 0};
                }
                if (!count)
                {
                    return {Status::Success, 0};
                }
                return bytes_ ? bytes_->exchange_bytes(tx, rx, count, timeout_ms)
                              : Result{Status::Unsupported, 0};
            }

            /** @brief Exchanges one word, changing rx only on success. */
            Status exchange_word(Word tx, Word &rx, uint32_t timeout_ms)
            {
                Word received = 0;
                const Result result = exchange(&tx, &received, 1, timeout_ms);
                if (result.status == Status::Success)
                {
                    rx = received;
                }
                return result.status;
            }

        private:
            Controller &controller_;
            ByteExchange *bytes_;
        };
    }
}

#endif
