# gmsv_mongodb

## Installing

To install this module, you can either build it from source or grab a pre-built
binary from the "Releases" section.

Place the built/downloaded `.dll` in `garrysmod/lua/bin`.

## Building

`gmsv_mongodb` requires the MongoDB C driver.

I recommend using [vcpkg](https://github.com/microsoft/vcpkg) for installing these
packages as it streamlines the process.

These packages can be installed using `vcpkg install mongo-c-driver`.

If you are on windows, ensure your default and target triplet are set to static (`x64-windows-static`).


