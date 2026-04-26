# Computer Graphics – AIUB visualization

This is an OpenGL/GLUT project that lets you change the environment of three buildings using keyboard keys.

## Project Environment
- **IDE**: Code::Blocks  
- **Graphics Library**: GLUT (FreeGLUT)  
- **Language**: C++  

## D Building
| Effect          | Key to Press |
|----------------|--------------|
| Day mode        | `D` or `d`   |
| Night mode      | `D` or `d`   |
| Rain effect     | `R` or `r`   |
| Flower / garden | `S` or `s`   |

> *Note: If your implementation toggles day/night with the same key, press `D`/`d` repeatedly.*

## C Building
| Effect          | Key to Press |
|----------------|--------------|
| Day mode        | `D` or `d`   |
| Night mode      | `N` or `n`   |
| Rainy effect    | `R` or `r`   |

## AIUB visualization in graphic design
| Effect          | Key to Press |
|----------------|--------------|
| Day mode        | `A` or `a`   |
| Night mode      | `A` or `a` |

## General Instructions
- Press the keys **while the graphics window is active** (click on the window first).
- Some effects may be **toggles** (press once for day, again for night).
- Rain/Flower effects may be **independent** of day/night.
- If nothing happens, make sure your keyboard **Caps Lock is off** (lowercase usually works too).

## How to Run in Code::Blocks
1. Open the `.cbp` file (`AIUB visualization in graphic design.cbp`).
2. Make sure GLUT is linked:  
   `Settings` → `Compiler` → `Linker settings` → Add `-lopengl32 -lglu32 -lfreeglut` (Windows) or as per your setup.
3. Build and run (F9 or `Build → Run`).

## Project Contents
- `main.cpp` – the GLUT source code
- `AIUB visualization in graphic design.cbp` – Code::Blocks project file
- `bin/` – compiled executable
- `obj/` – object files
