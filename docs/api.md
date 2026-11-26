# Use the SPI Core

Public entry headers are [spi/spi.hpp](../include/spi/spi.hpp).

## Configure before exchanging

Construct `SPI` from `Controller&` for words or from `ByteExchange&` to retain the optional packed-buffer path. `configure` checks width and the role-specific clock rate, then asks the Adapter to accept the format. Arrange chip select through the caller or target composition.

Use `exchange_word` to receive one word with output preservation on failure. Use `exchange` for word buffers and `exchange_bytes` for packed bytes on an eight-bit capable binding. A valid empty transfer succeeds without dispatch. A nonempty transfer with missing buffers or a zero timeout fails locally. A missing byte facet produces `Unsupported` for a valid nonempty request.

## Handle short transfers

Check `status` and `completed` before using output. A partially completed exchange may already have affected the peer. The Core neither retries nor controls chip select.

## Composition and lifetime

Application code calls the SPI Core. It borrows the matching Contract, which an Adapter implements using its Hardware Definitions. Future Devices will create or bind these objects and establish their lifetime. To use the examples today, construct the facade directly with the supplied fake. Keep each borrowed resource alive for its use by the facade and follow the Adapter's access and synchronization requirements.
