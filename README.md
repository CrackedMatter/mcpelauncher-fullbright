# mcpelauncher-fullbright

A mod for mcpelauncher that increases brightness.


## Installation

Create a `mods` directory in the directory of your mcpelauncher profile (e.g. `~/.local/share/mcpelauncher`) if you have not already done so.
Download the mod from [releases](https://github.com/CrackedMatter/mcpelauncher-fullbright/releases) and move the file into the `mods` directory.


## Usage

- Increase the brightness value in the game's video settings
- Disable vibrant visuals


## Building

Prerequisites:

- Android NDK r27 or later. [Download](https://developer.android.com/ndk/downloads)
- CMake 3.20 or later

Replace `/path/to/ndk` with the actual path to the Android NDK:

```
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/ndk/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```

This will create a `build` directory containing the mod file. Install it as described in [Installation](#installation).
