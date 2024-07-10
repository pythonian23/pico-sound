all: format build flash

format:
    indent -linux *.c *.h

build:
    cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
    cp build/compile_commands.json ./
    cd build && make -j16

flash:
    sudo picotool load ./build/pico-sound.uf2
    sudo picotool reboot
