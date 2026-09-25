# Adding a Game

## Prerequisites

- Understand the `IGameModule` interface (see `include/IGameModule.hpp`)
- Understand the `RenderData` format (see [04_render_data.md](./04_render_data.md))

---

## Step 1 — Create source files

```
src/
└── games/
    └── MyGame/
        ├── MyGame.hpp
        └── MyGame.cpp
```

---

## Step 2 — Implement `IGameModule`

### MyGame.hpp

```cpp
#pragma once
#include "IGameModule.hpp"
#include <string>

class MyGame : public Arcade::IGameModule {
public:
    MyGame();
    ~MyGame() override = default;

    void              init()                         override;
    void              reset()                        override;
    void              close()                        override;
    void              handleInput(Arcade::Key key)   override;
    void              update()                       override;
    const Arcade::RenderData &getRenderData()        override;
    bool              isGameOver()  const            override;
    int               getScore()   const             override;
    const std::string &getName()   const             override;

private:
    std::string        _gameName;
    Arcade::RenderData _renderData;
    bool               _gameOver;
    int                _score;
    int                _gridWidth;
    int                _gridHeight;

    // Your game state here
    void buildRenderData();
};
```

### MyGame.cpp — minimal skeleton

```cpp
#include "MyGame.hpp"

MyGame::MyGame()
    : _gameName("MyGame"), _gameOver(false), _score(0),
      _gridWidth(20), _gridHeight(20)
{}

void MyGame::init() {
    _gameOver = false;
    _score    = 0;
    // Initialize your game state
    buildRenderData();
}

void MyGame::reset() {
    init();
}

void MyGame::close() {
    // Free any resources
}

void MyGame::handleInput(Arcade::Key key) {
    if (key == Arcade::Key::UP)    { /* move up    */ }
    if (key == Arcade::Key::DOWN)  { /* move down  */ }
    if (key == Arcade::Key::LEFT)  { /* move left  */ }
    if (key == Arcade::Key::RIGHT) { /* move right */ }
}

void MyGame::update() {
    if (_gameOver) return;
    // Game logic: move entities, detect collisions…
    // On losing condition:
    //   _gameOver = true;
    buildRenderData();
}

const Arcade::RenderData &MyGame::getRenderData() {
    return _renderData;
}

bool MyGame::isGameOver() const {
    return _gameOver;
}

int MyGame::getScore() const {
    return _score;
}

const std::string &MyGame::getName() const {
    return _gameName;
}

// ─── Build RenderData ─────────────────────────────────────────────────────────

void MyGame::buildRenderData() {
    _renderData.gridWidth  = _gridWidth;
    _renderData.gridHeight = _gridHeight;

    // Fill the grid
    _renderData.grid.clear();
    for (int y = 0; y < _gridHeight; ++y) {
        std::vector<Arcade::Cell> row;
        for (int x = 0; x < _gridWidth; ++x) {
            Arcade::Cell cell;
            cell.character = ' ';
            cell.fgColor   = Arcade::Color::WHITE;
            cell.bgColor   = Arcade::Color::BLACK;
            row.push_back(cell);
        }
        _renderData.grid.push_back(row);
    }

    // Display score as text
    _renderData.texts.clear();
    Arcade::Text scoreText;
    scoreText.content = "Score: " + std::to_string(_score);
    scoreText.x       = 0;
    scoreText.y       = _gridHeight + 1;
    scoreText.color   = Arcade::Color::WHITE;
    _renderData.texts.push_back(scoreText);
}
```

---

## Step 3 — Export the C symbol (mandatory)

```cpp
// At the end of MyGame.cpp
extern "C" Arcade::IGameModule *createGameModule() {
    return new MyGame();
}
```

> **Important**: the symbol name must be exactly `createGameModule`.

---

## Step 4 — Compile as a `.so`

### With g++ directly

```bash
g++ -shared -fPIC -o lib/arcade_mygame.so \
    src/games/MyGame/MyGame.cpp \
    -I include/
```

### With a Makefile (example)

```makefile
LIB_NAME = lib/arcade_mygame.so
SRC      = src/games/MyGame/MyGame.cpp
CXXFLAGS = -std=c++20 -Wall -Wextra -fPIC -I include/
LDFLAGS  = -shared

$(LIB_NAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
```

---

## Step 5 — Place in `./lib/`

The filename must **not** contain `ncurses`, `sdl2`, or `sfml` (those keywords identify graphical libraries).

```bash
cp lib/arcade_mygame.so ./lib/
```

The Core will detect it automatically on the next launch.

---

## Game Over Handling

When `isGameOver()` returns `true`, the Core:
1. Records the score via `ScoreManager` (using `getName()` and `getScore()`)
2. Returns to the menu

Your game **must not** handle the return to menu itself.

---

## Summary — Checklist

- [ ] Class inheriting from `IGameModule`
- [ ] All interface methods implemented
- [ ] `getRenderData()` returns a valid `RenderData` every frame
- [ ] `isGameOver()` returns `true` when the game is over
- [ ] C symbol `createGameModule` exported with `extern "C"`
- [ ] Compiled with `-shared -fPIC`
- [ ] The `.so` file is in `./lib/` without graphical keywords in the name