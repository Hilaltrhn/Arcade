# RenderData — Game ↔ Display Communication

`RenderData` is the **only structure exchanged** between a game (`IGameModule`) and a graphical library (`IDisplayModule`). It is defined in `include/RenderData.hpp` (or equivalent).

---

## Full Structure

```cpp
namespace Arcade {

    // Available colors
    enum class Color {
        BLACK,
        WHITE,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN
    };

    // A single cell in the game grid
    struct Cell {
        char  character;  // Character to display ('o', '#', ' ', …)
        Color fgColor;    // Foreground (character) color
        Color bgColor;    // Background color
    };

    // A text element displayed outside the grid (HUD, score, game over…)
    struct Text {
        std::string content;  // Text content
        int         x;        // Column (in character units)
        int         y;        // Row
        Color       color;    // Text color
    };

    // Main structure exchanged between game and display
    struct RenderData {
        int                            gridWidth;   // Grid width
        int                            gridHeight;  // Grid height
        std::vector<std::vector<Cell>> grid;        // Grid [y][x]
        std::vector<Text>              texts;       // Text overlays
    };

}
```

---

## The Grid (`grid`)

The grid is a 2D vector of `Cell`, indexed as `grid[y][x]`.

- The **Y** axis goes from top (0) downward.
- The **X** axis goes from left (0) rightward.
- Each cell holds a character and its colors.

### Example — Snake

| Character | Meaning | Color |
|---|---|---|
| `#` | Wall | `BLUE` |
| `@` | Snake head | `YELLOW` |
| `o` | Snake body | `GREEN` |
| `*` | Food | `RED` |
| ` ` | Empty cell | `WHITE` |

```cpp
// Place a wall along the top of the grid
for (int x = 0; x < gridWidth; ++x) {
    _renderData.grid[0][x].character = '#';
    _renderData.grid[0][x].fgColor   = Arcade::Color::BLUE;
}
```

---

## Texts (`texts`)

Texts allow you to display the HUD (score, game name, game over…) **below** or **overlaid on** the grid.

Position `(x, y)` is in **character units**. By convention, place texts below the grid:

```cpp
Arcade::Text scoreText;
scoreText.content = "Score: " + std::to_string(_score);
scoreText.x       = 0;
scoreText.y       = _gridHeight + 1; // just below the grid
scoreText.color   = Arcade::Color::WHITE;
_renderData.texts.push_back(scoreText);
```

---

## RenderData Lifecycle

```
┌─────────────────────────────┐
│      IGameModule            │
│                             │
│  update()                   │
│    └─► buildRenderData()    │  ← fills grid + texts
│                             │
│  getRenderData()  ──────────┼──► RenderData (const ref)
└─────────────────────────────┘              │
                                             ▼
                              ┌──────────────────────────┐
                              │     IDisplayModule        │
                              │                           │
                              │  render(data)             │
                              │    ├─ renderGrid(data)    │
                              │    └─ renderTexts(data)   │
                              │                           │
                              │  display()                │
                              └──────────────────────────┘
```

1. The game updates its internal state in `update()`.
2. It encodes that state into `_renderData` via `buildRenderData()`.
3. The Core calls `getRenderData()` → the graphical library receives a `const ref`.
4. The graphical library iterates over `grid` and `texts` to draw.
5. `display()` presents the frame on screen.

---

## Implementation Tips

- **Rebuild `_renderData` on every `update()`**: it is simple and avoids hidden state.
- **Do not store pointers** to `RenderData` inside the graphical library: it is a temporary reference valid only until the next `update()`.
- **Always clear the entire grid** with spaces before drawing entities, otherwise artifacts from the previous frame may persist.

```cpp
// Proper grid initialization
void MyGame::buildRenderData() {
    _renderData.gridWidth  = _gridWidth;
    _renderData.gridHeight = _gridHeight;
    _renderData.grid.clear();
    _renderData.texts.clear();

    for (int y = 0; y < _gridHeight; ++y) {
        std::vector<Arcade::Cell> row(_gridWidth);
        for (auto &cell : row) {
            cell.character = ' ';
            cell.fgColor   = Arcade::Color::WHITE;
            cell.bgColor   = Arcade::Color::BLACK;
        }
        _renderData.grid.push_back(row);
    }

    // ... draw entities on top ...
}
```