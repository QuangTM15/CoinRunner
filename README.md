# 🎮 CoinRunner

**CoinRunner** is a small 2D platformer game made with **C++** and **SFML 3**.

The goal is simple:  
run, jump, collect coins, avoid traps, and reach the goal of each level.

This project was created as a **learning-focused indie game**, with an emphasis on:
- clean scene flow
- simple but solid gameplay
- proper game states (menu, pause, game over, ending)

---

## ✨ Features

- 2D platformer gameplay
- Player movement, jump, gravity, and collision
- Coins and traps (static & moving)
- Kill zones and checkpoints
- Life system
- HUD (Life & Coin)
- Pause menu (ESC)
- Level Select with unlock system
- Save system (unlock progress only)
- Game Over screen
- Level Complete screen
- Fake End Game sequence with music and timed text

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| A / D | Move left / right |
| ← / → | Move left / right |
| W / ↑ | Jump |
| Space | Jump |
| ESC | Pause |
| Mouse | Menu interaction |

---

## 📂 Project Structure

CoinRunner/  
├─ asset/  
│  ├─ maps/  
│  ├─ textures/  
│  ├─ ui/  
│  ├─ sound/  
│  └─ musics/  
│  
├─ src/  
│  ├─ core/  
│  ├─ scene/  
│  ├─ system/  
│  └─ ui/  
│  
├─ include/  
├─ build/  
└─ README.md  

---

## 💾 Save System

The game automatically saves progress to:

**%LOCALAPPDATA%/CoinRunner/save.json**

Saved data example:

```json
{
  "maxUnlockedLevel": 2
}
```
Only level unlock progress is saved.
Coins, lives, and checkpoints are not saved by design.

---

## 🔧 Build & Run

Requirements
C++17 compatible compiler

SFML 3.0.2

MinGW / g++


```bash
g++ src/main.cpp src/core/*.cpp src/system/*.cpp src/scene/*.cpp src/ui/*.cpp \
-Iinclude -I<SFML_INCLUDE_PATH> \
-L<SFML_LIB_PATH> \
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
-std=c++17 -O2 -o CoinRunner.exe
```
Or simply run the provided executable inside the build folder.
---

## 🎵 Assets & Credits
Fonts, sounds, and textures are used for educational purposes.
All assets belong to their respective creators.

---

## 🚀 Project Status
✔ Core gameplay complete

✔ UI & scene flow complete

✔ Save system implemented

✔ End game implemented

This project is considered finished and is not planned for further expansion.

---

## 👤 Author
Developed by a solo developer as a learning and indie game project.

Enjoy the game! 🎉