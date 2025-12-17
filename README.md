# Tron CLI Game

A terminal-based Tron light cycle game built with C++ and ncurses.

This project was born out of pure boredom during university lectures. I decided to build something fun while practicing my C++ skills.

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
sudo apt-get install libncurses5-dev libncurses-dev

# Install UTF-8 locales
sudo apt-get install locales
sudo locale-gen en_US.UTF-8
```

### Build & Run

```bash
# Clone the repository
git clone https://github.com/notCienki/tron-cli-game
cd tron-cli-game

# Build and run using Makefile
make
./tron

# Or build and run in one command
make run
```

**Installation (optional):**
```bash
# Install to /usr/local/bin
sudo make install

# Or install to custom location
make install PREFIX=~/.local

# Then run from anywhere
tron
```

**Other commands:**
```bash
make clean      # Remove build files
make help       # Show all available commands
```
