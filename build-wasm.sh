#!/bin/bash
set -euo pipefail

# Get EMSDK on the PATH
cd ../emsdk
source emsdk_env.sh

# Get into the /src folder
cd ../Examensarbete_3.0
cd src

# Build to Web Assembly
emcc -o ../out/index.html \
    main.c -Os -Wall /Users/joshua.fourie/Documents/Github/raylib/build_wasm/raylib/libraylib.a \
    -I. -I /Users/joshua.fourie/Documents/Github/raylib/build_wasm/raylib/include \
    -L. -L /Users/joshua.fourie/Documents/Github/raylib/build_wasm/raylib -lraylib \
    -s USE_GLFW=3 \
    -s ASYNCIFY \
    --shell-file ../shell.html \
    --preload-file Sprites \
    -s TOTAL_STACK=64MB \
    -s INITIAL_MEMORY=128MB \
    -s ASSERTIONS \
    -DPLATFORM_WEB

# Get into /out
cd ../out

# Run the game
emrun index.html