# **Socket CLI Bowling:** <!-- omit in toc -->
> <sup>A two-player socket-based bowling game implemented in C and Raylib.</sup>

### **Team:**
- [**Noel Emaas**](https://github.com/NoelEmaas)
- [**Albert Perez**](https://github.com/bibookss)
- [**Jiankarlo Belarmino**](https://github.com/CSjianbel)
- [**Martin Edgar Atole**](https://github.com/CS-Martin)

## **How to use**
  - You need a GCC (or alternative C99 compiler), make and git (to download raylib repo).
    ```
    You need a GCC (or alternative C99 compiler), make and git (to download raylib repo).
    ```
  - Optionally, you could use CMake building system.
    ```
    sudo apt install cmake
    ```
  - Install required libraries
    You need to install some required libraries; ALSA for audio, Mesa for OpenGL accelerated graphics and X11 for windowing system.

    Ubuntu
    ```
    sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
    ```

    Fedora
    ```
    sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
    ```

    Arch Linux
    ```
    sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
    ```
  - Download the raylib repository from Github, then compile it with:
    ```
    git clone https://github.com/raysan5/raylib.git raylib
    cd raylib/src/
    make PLATFORM=PLATFORM_DESKTOP # To make the static version.
    ```
  - Lastly, compile the program
    ```
    cd /bowling
    ./compile.sh
    ./a.out
    ```


    

