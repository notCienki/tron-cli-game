# 🏍️ Tron CLI Game

A terminal-based Tron light cycle game built with C++ and ncurses.


This project was born out of pure boredom during university lectures. Instead of scrolling through social media, I decided to build something fun while practicing my C++ skills.

Classic Tron gameplay: control your light cycle, avoid crashing into walls and trails, make your opponents crash first.

## Quick Start

**Prerequisites:**
```bash
# macOS
brew install ncurses

# Linux
sudo apt-get install libncurses5-dev
```

**Build & Run:**
```bash
git clone https://github.com/yourusername/tron-cli.git
cd tron-cli
make
./tron
```

## Controls

- **Arrow Keys** - Move your light cycle
- **Q/ESC** - Quit game

## Structure

```
tron-cli/
├── include/     # Headers (game.h, player.h)
├── src/         # Source files
├── Makefile     # Build config
└── README.md
```

## Learning Goals

- Object-Oriented Programming in C++
- Game loops and real-time input handling
- Terminal programming with ncurses
- Proper project organization


## 🤝 Contributing

Bug fixes, features, and improvements welcome! Fork, code, and PR.

---

*Built with ❤️ and caffeine during boring lectures*