# Adding a Graphical Library

## Prerequisites

- Understand the `IDisplayModule` interface (see `include/IDisplayModule.hpp`)
- Have a compatible build system (Makefile, CMake…)
- The library must compile as a shared `.so`

---

## Step 1 — Create source files

```
src/
└── display/
    └── MyLib/
        ├── MyLibDisplay.hpp
        └── MyLibDisplay.cpp
```

---

## Step 2 — Implement `IDisplayModule`

### MyLibDisplay.hpp

```cpp
#pragma once
#include "IDisplayModule.hpp"
#include <string>

class MyLibDisplay : public Arcade::IDisplayModule {
public:
    MyLibDisplay();
    ~MyLibDisplay() override = default;

    void        init()    override;
    void        close()   override;
    void        clear()   override;
    void        render(const Arcade::RenderData &data) override;
    void        display() override;
    Arcade::Key pollEvent() override;
    const std::string &getName() const override;

private:
    std::string _name;
    // your private members here (window, renderer, etc.)
};
```

### MyLibDisplay.cpp — minimal skeleton

```cpp
#include "MyLibDisplay.hpp"

MyLibDisplay::MyLibDisplay() : _name("mylib") {}

void MyLibDisplay::init() {
    // Open the window, initialize the graphical context
}

void MyLibDisplay::close() {
    // Clean window shutdown
}

void MyLibDisplay::clear() {
    // Clear the current buffer
}

void MyLibDisplay::render(const Arcade::RenderData &data) {
    // Iterate over data.grid and data.texts, draw each element
    for (int y = 0; y < data.gridHeight; ++y) {
        for (int x = 0; x < data.gridWidth; ++x) {
            const Arcade::Cell &cell = data.grid[y][x];
            // Draw cell.character at position (x, y)
            // using cell.fgColor as foreground color
        }
    }
    for (const auto &text : data.texts) {
        // Draw text.content at (text.x, text.y)
        // using text.color as color
    }
}

void MyLibDisplay::display() {
    // Buffer swap / refresh / present frame
}

Arcade::Key MyLibDisplay::pollEvent() {
    // Read keyboard event and translate it to Arcade::Key
    // Return Arcade::Key::NONE if no event is pending
    return Arcade::Key::NONE;
}

const std::string &MyLibDisplay::getName() const {
    return _name;
}
```

---

## Step 3 — Export the C symbol (mandatory)

The Core looks for the symbol `createDisplayModule` via `dlsym`.  
You **must** export it with `extern "C"` to prevent C++ name mangling:

```cpp
// At the end of MyLibDisplay.cpp (or in a dedicated file)
extern "C" Arcade::IDisplayModule *createDisplayModule() {
    return new MyLibDisplay();
}
```

> **Important**: the symbol name must be exactly `createDisplayModule`.

---

## Step 4 — Compile as a `.so`

### With g++ directly

```bash
g++ -shared -fPIC -o lib/arcade_mylib.so \
    src/display/MyLib/MyLibDisplay.cpp \
    -I include/ \
    -lmylib_dependency   # add your graphical library flags here
```

### With a Makefile (example)

```makefile
LIB_NAME = lib/arcade_mylib.so
SRC      = src/display/MyLib/MyLibDisplay.cpp
CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC -I include/
LDFLAGS  = -shared -lmylib_dependency

$(LIB_NAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
```

---

## Step 5 — Place in `./lib/`

The filename **must** contain one of the following keywords to be recognized as a graphical library (rather than a game):

- `ncurses`
- `sdl2`
- `sfml`

```bash
cp lib/arcade_mylib.so ./lib/arcade_sdl2.so   # example for SDL2
```

If you add a new technology, update `scanLibraries()` in `Core.cpp` to include your keyword.

---

## Summary — Checklist

- [ ] Class inheriting from `IDisplayModule`
- [ ] All interface methods implemented
- [ ] C symbol `createDisplayModule` exported with `extern "C"`
- [ ] Compiled with `-shared -fPIC`
- [ ] The `.so` file is placed in `./lib/`
- [ ] The filename contains `ncurses`, `sdl2`, or `sfml`