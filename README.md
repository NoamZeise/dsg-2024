# Orbbit

Launch your green satellite to the red orb to complete the level.

Made in 44 hours for [Duck Sauce Games 2024](https://www.ducksauce.games/duck-sauce-jam-2024)
Theme: Discovery


Made using my [graphics library](https://github.com/NoamZeise/Graphics-Environment).


# Build Instructions

Requires Vulkan Headers, CMake, C++ compiler (tested with gcc).

git clone this repo with submodules
```
git clone --recurse-submodules https://github.com/NoamZeise/dsg-2024.git
```

build in Release mode with apropriate flags
```
cd dsg-2024
mkdir build && cd build
cmake .. -DNO_AUDIO=true -DNO_ASSIMP=true -DNO_FREETYPE=true -DGRAPHICS_BUILD_STATIC=true -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

The executable should now be in the `src/` folder of the build directory
