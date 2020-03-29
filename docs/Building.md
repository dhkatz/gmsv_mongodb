# Building

`gmsv_mongodb` requires the MongoDB C driver.

I recommend using [vcpkg](https://github.com/microsoft/vcpkg) for installing these
packages as it streamlines the process.

Building `gmsv_mongodb` requires at least GCC 7 if building on Linux
 and CMake 3.13+ on all platforms.

Ensure `VCPKG_DEFAULT_TRIPLET` and `VCPKG_TARGET_TRIPLET` are set to the proper values
for your platform.
 
 Follow the instructions for setting up `vcpkg` and then install the Mongo driver
 
 `vcpkg install  mongo-c-driver`
 
 Then inside the repo create a build directory and enter it
  
  `mkdir build && cd build`
 
 Then create the CMake project

`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=<PATH TO TOOLCHAIN> -DVCPKG_TARGET_TRIPLET=<YOUR TARGET TRIPLET> ..`

Finally build the project

**Windows**

`cmake --build . --target ALL_BUILD --config Release`

**Linux**

`make -j`