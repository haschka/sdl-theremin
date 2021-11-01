# SDL-Theremin

(c) Thomas Haschka 2021

The Theremin in multitouch as shown in
[https://youtu.be/RnecZHtqQF4](https://youtu.be/RnecZHtqQF4)

# Usage:

Install the SDL2 library and its headers i.e. SDL2-dev

Compile with:
```
gcc sdl-theremin.c `sdl2-config --libs` `sdl2-config --cflags` -lm -o sdl-theremin
```
Run:
```
./sdl-theremin
```
Use two fingers in the window appearning on the screen. Rotation controls gain, Distance frequency.

# License 
```
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
```
