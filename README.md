<h1 align="center">Tron CLI</h1>

![Gameplay Demo](demo.gif)

Terminal-based Tron light cycle game with intelligent bot opponent.

Built with C++ and ncurses. Born from university lecture boredom.

## Prerequisites

**macOS:**
```bash
# Install ncurses via Homebrew
brew install ncurses

# Ensure UTF-8 support (for fancy borders)
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8
```

**Linux:**
```bash
# Install ncurses development package
sudo apt-get install libncurses5-dev libncurses-dev

# Install UTF-8 locales
sudo apt-get install locales
sudo locale-gen en_US.UTF-8
```

## Build & Run

```bash
# Clone the repository
git clone https://github.com/notCienki/tron-cli
cd tron-cli

# Build and run
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

## Bot Features

Bot uses hybrid decision-making algorithm:
- Flood-fill space analysis
- Multi-criteria evaluation
- 10-step look-ahead simulation
- Trap detection and avoidance

## License

MIT
