# Tron CLI Game

A terminal-based Tron light cycle game built with C++ and ncurses.

This project was born out of pure boredom during university lectures. Instead of scrolling through social media, I decided to build something fun while practicing my C++ skills.

Classic Tron gameplay: control your light cycle, avoid crashing into walls and trails, make your opponents crash first.

## Quick Start

### Prerequisites

**macOS:**
```bash
# Install ncurses via Homebrew
brew install ncurses

# Ensure UTF-8 support (for fancy borders)
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8
```

**Linux :**
```bash
# Install ncurses development package
sudo apt-get install libncurses5-dev

# Install UTF-8 locales
sudo apt-get install locales
sudo locale-gen en_US.UTF-8
```

### Build & Run

```bash
git clone https://github.com/notCienki/tron-cli-game
cd tron-cli
g++ -std=c++17 -Wall -g -o tron src/main.cpp src/game.cpp src/player.cpp -lncurses
./tron
```
## Controls

- **↑↓←→** - Move your light cycle
- **Q/ESC** - Quit game  
- **R** - Restart after game over

## Structure

```
tron-cli/
├── include/     # Headers (game.h, player.h)
├── src/         # Source files
├── Makefile (not implemented yet)     # Build config
└── README.md
```
