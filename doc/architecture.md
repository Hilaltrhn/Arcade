# Global Architecture

## Overview

The Arcade project is built on three distinct layers that communicate through abstract C++ interfaces and dynamic library loading (`.so`).

```
┌──────────────────────────────────────────────────────┐
│                        Core                          │
│  - Main loop                                         │
│  - Menu management                                   │
│  - ScoreManager                                      │
│  - DLLoader<IDisplayModule> / DLLoader<IGameModule>  │
└────────────────┬─────────────────┬───────────────────┘
                 │                 │
        ┌────────▼──────┐  ┌───────▼────────┐
        │ IDisplayModule│  │  IGameModule   │
        │  (interface)  │  │  (interface)   │
        └────────┬──────┘  └───────┬────────┘
                 │                 │
        ┌────────▼──────┐  ┌───────▼────────┐
        │  ncurses.so   │  │   snake.so     │
        │   sdl2.so     │  │  mygame.so     │
        │   sfml.so     │  │   ...          │
        └───────────────┘  └────────────────┘
```

## The Core

`Arcade::Core` is the orchestrator. It:

1. **Scans** `./lib/` at startup to list available `.so` files (graphical libs are identified by the keywords `ncurses`, `sdl2`, `sfml`; everything else is treated as a game).
2. **Dynamically loads** the initial graphical library passed as a command-line argument.
3. **Drives the main loop**: poll events → update game → render.
4. **Allows hot-switching** of graphical library or game at runtime (`NEXT_LIB`, `PREV_LIB`, `NEXT_GAME`, `PREV_GAME` keys).

```cpp
// Core.cpp — simplified main loop
while (_running) {
    Key key = _graphicalModule->pollEvent();
    handleSystemEvents(key);
    _gameModule->handleInput(key);
    _gameModule->update();
    _graphicalModule->clear();
    _graphicalModule->render(_gameModule->getRenderData());
    _graphicalModule->display();
}
```

## DLLoader

`DLLoader<T>` is a wrapper around `dlopen` / `dlsym` / `dlclose`.  
It loads a `.so` library and extracts a C symbol (factory function) to instantiate the module.

```cpp
// Loading a graphical library
_graphicalLoader = std::make_unique<DLLoader<IDisplayModule>>(path);
_graphicalModule  = _graphicalLoader->getInstance("createDisplayModule");

// Loading a game
_gameLoader = std::make_unique<DLLoader<IGameModule>>(path);
_gameModule  = _gameLoader->getInstance("createGameModule");
```

## Interfaces

### `IDisplayModule`

Represents a graphical library. Required methods:

| Method | Role |
|---|---|
| `init()` | Window / context initialization |
| `close()` | Clean shutdown |
| `clear()` | Clear the screen |
| `render(RenderData)` | Draw the current frame |
| `display()` | Present frame (buffer swap / refresh) |
| `pollEvent()` | Read input → return `Arcade::Key` |
| `getName()` | Library name |

### `IGameModule`

Represents a game. Required methods:

| Method | Role |
|---|---|
| `init()` | Initialize game state |
| `reset()` | Reset to initial state |
| `close()` | Clean up resources |
| `handleInput(Key)` | Receive a key press |
| `update()` | Advance one game tick |
| `getRenderData()` | Return the current `RenderData` |
| `isGameOver()` | `true` when the game is over |
| `getScore()` | Current score |
| `getName()` | Game name |

## Frame Lifecycle

```
pollEvent()  ──► Key
                  │
          ┌───────▼──────────┐
          │ handleSystemEvents│  (switch lib, switch game, quit…)
          └───────┬──────────┘
                  │
          ┌───────▼──────────┐
          │ game.handleInput  │
          └───────┬──────────┘
                  │
          ┌───────▼──────────┐
          │   game.update()   │
          └───────┬──────────┘
                  │
          ┌───────▼──────────┐
          │ getRenderData()   │──► RenderData
          └───────────────────┘         │
                                ┌───────▼──────────┐
                                │  display.render() │
                                └───────────────────┘
```

## Reserved System Keys

| Key | Action |
|---|---|
| `ESCAPE` | Return to menu |
| `QUIT` | Exit the program |
| `NEXT_LIB` / `PREV_LIB` | Switch graphical library |
| `NEXT_GAME` / `PREV_GAME` | Switch game |
| `RESTART` | Restart the current game |