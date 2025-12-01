# CoinRunner (SFML 3)

A 2D platformer game built with **C++ + SFML 3**, inspired by Ninja School and classic coin-collecting platformers.

---

## 📦 Features
- Tile-based map loaded from JSON
- Player movement + jumping physics
- Coins, obstacles, traps
- Camera following player
- Multiple levels with different themes

---

## 🛠 Requirements
- C++17 or newer
- CMake 3.25+
- SFML 3.0

---

## 📚 How to Build

### 1. Install SFML 3  
Download from official repository.

### 2. Configure project

cmake -B build -DCMAKE_PREFIX_PATH="path/to/sfml"


### 3. Build

cmake --build build --config Release


Executable will be inside:

build/

---

## 📁 Project Structure

CoinRunner/
│── assets/
│── src/
│ ├── Game.cpp/hpp
│ ├── Player.cpp/hpp
│ ├── TileMap.cpp/hpp
│ ├── Coin.cpp/hpp
│ ├── Obstacle.cpp/hpp
│ └── main.cpp
│── CMakeLists.txt
│── .gitignore
│── README.md

---

## ✨ Author
CoinRunner project by To Minh Quang – learning & building game development step by step!