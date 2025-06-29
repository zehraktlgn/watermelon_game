# Watermelon Slicing Game

A simple Qt-based game where the player slices watermelons by clicking on them before they fall off the screen. The goal is to slice as many watermelons as possible within a limited time. Avoid clicking on bomb watermelons, as doing so ends the game immediately.

## Features

- Creates watermelons every 0.5 seconds.
- Game duration is limited to 30 seconds.
- Keeps track of sliced and missed watermelons.
- Saves and compares scores using a local file (`skor123.txt`).
- Randomly generates "bomb" watermelons that end the game if clicked.
- Watermelon positions are loaded from a text file (`konumlar.txt`).
- Uses Qt's signal-slot mechanism for all interactions.

## Requirements

- Qt 5 or later
- A C++ compiler (e.g. MSVC, MinGW)
- Qt Creator (recommended for easier setup)

## Build Instructions

1. Clone or download the repository.
2. Open the project in Qt Creator (`.pro` file).
3. Click **Build** and then **Run**.

Or from terminal (if `.pro` is available):

```bash
qmake
make
