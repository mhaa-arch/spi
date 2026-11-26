# MHAA SPI Core

Use `SPI` to set role, clock rate, mode, order and width, then exchange one word or a buffer. The Core checks basic request shapes and exposes a packed-byte path for a supporting controller.

Check `configure` before calling `exchange_word`, `exchange` or the optional `exchange_bytes` path. Keep chip select in the caller or composition.

## Use in an application

Add this repository's `include/` and the matching `spi-contract/include/` to the C++11 include path. Include `<spi/spi.hpp>` and supply a live implementation of the matching Contract. The Core borrows it. In a future device composition, Devices will select the concrete Adapter and wire it to the Core.

- [Technical guide](docs/index.md)
- [Core usage](docs/api.md)
- [Portability and limits](docs/compatibility.md)
- [Worked example and build commands](docs/examples.md)
- [MIT License](LICENSE)

## Generate the API reference

Use Doxygen 1.18.0. From this repository root, create `build/` if it does not exist, then run `doxygen Doxyfile`.

```sh
mkdir -p build
doxygen Doxyfile
```

On PowerShell, use `New-Item -ItemType Directory -Force build` for the first command. Open `build/docs/html/index.html`. Mermaid diagrams render in a browser with access to the Mermaid CDN. GitHub Pages publication runs on published releases and on manual dispatches from `main` once Pages is enabled.
