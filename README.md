# Arcade - Documentation

Welcome to the Arcade project documentation.
This folder contains everything you need to understand the architecture and extend the project with new modules.

## Table of Contents

| File | Content |
|---|---|
| [01_architecture.md](./01_architecture.md) | Global architecture: Core, interfaces, dynamic loading |
| [02_add_graphical_lib.md](./02_add_graphical_lib.md) | Step-by-step guide: create a graphical library |
| [03_add_game.md](./03_add_game.md) | Step-by-step guide: create a game |
| [04_render_data.md](./04_render_data.md) | RenderData description and game/display communication |
| [05_uml_diagram.md](./05_uml_diagram.md) | UML class diagram |

## Quick Start

Launch the arcade with a graphical library:

```bash
./arcade ./lib/arcade_ncurses.so
```

Graphical libraries and games are automatically detected from `./lib/`.