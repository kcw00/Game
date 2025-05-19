# 🏓 Pong game (Built from scratch in 2022)

A classic Pong-style game built with **C++** and **SDL2** libraries. The player controls the paddle using the keyboard to keep the ball in play

---
### 📸 Live Demo

![ScreenRecording2025-05-18at18 32 32-ezgif com-video-to-gif-converter (1)](https://github.com/user-attachments/assets/9f3f1eb9-15aa-4624-a2f6-3cbd426f0f40)

---
### ✨ Features
- Real-time paddle and ball interaction
- Ball color switches on wall collision
- Live score display
- Background music and sound effects
- Clean and minimal interface

---

### ⚙️ Set up (macOS Apple Silicon)

#### ✅ Prerequisites
Make sure you have these installed via [Homebrew](https://brew.sh/)
```bash
brew install sdl2 sdl2_ttf sdl2_mixer
```

1. Clone this repo
```bash
git clone https://github.com/kcw00/pong.git
cd pong-game
```
2. Compile the project
```bash
clang++ -std=c++17 Main.cpp Game.cpp -o pong \
  -I/opt/homebrew/include/SDL2 \
  -L/opt/homebrew/lib \
  -lSDL2 -lSDL2_ttf -lSDL2_mixer
```
3. Run the game
```bash
./pong
```
---
### 🕹 Controls

| Key    | Action             |
|--------|--------------------|
| `W`    | Move paddle up     |
| `S`    | Move paddle down   |
| `Z`    | Pause/Resume music |
| `ESC`  | Quit the game      |

