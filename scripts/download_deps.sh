if [ ! -d "libs/vectormath" ]; then
    git clone https://github.com/maihd/vectormath --recursive --depth=1 libs/vectormath
fi

if [ ! -d "libs/imgui" ]; then
    git clone https://github.com/ocornut/imgui --recursive --depth=1 libs/imgui
fi

if [ ! -d "libs/SDL3" ]; then
    git clone https://github.com/libsdl-org/SDL --recursive --depth=1 libs/SDL3
fi

if [ ! -d "libs/SDL3_image" ]; then
    git clone https://github.com/libsdl-org/SDL_image --recursive --depth=1 libs/SDL3_image
fi