# gmsv_mongodb

| Windows | Linux |
|---------|-------|
| [![Build status](https://ci.appveyor.com/api/projects/status/8ervbutov5w0144n/branch/development?svg=true)](https://ci.appveyor.com/project/dhkatz/gmsv-mongodb/branch/development)| [![Build Status](https://travis-ci.org/dhkatz/gmsv_mongodb.svg?branch=development)](https://travis-ci.org/dhkatz/gmsv_mongodb) |

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


