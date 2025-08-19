if not exist libs\vectormath (
    git clone https://github.com/maihd/vectormath --recursive --depth=1 libs/vectormath
)

if not exist libs\imgui (
    git clone https://github.com/ocornut/imgui --recursive --depth=1 libs/imgui
)

if not exist libs\SDL3 (
    git clone https://github.com/libsdl-org/SDL --recursive --depth=1 libs/SDL3
)

if not exist libs\SDL3_image (
    git clone https://github.com/libsdl-org/SDL_image --recursive --depth=1 libs/SDL3_image
)