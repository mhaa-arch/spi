# Using SPI across targets

The only project dependency is the matching `spi-contract`. Keep its API revision aligned with this Core.

Use word buffers across compatible targets with different native frame widths. Use the optional packed path when the target provides it and the link uses eight-bit words. Transfer length limits, clocks, pin mapping and actual bus timing still belong to the selected implementation.

This repository's host simulations and shared API compile/link checks provide source-level evidence. Target timing and physical behavior require hardware validation.
