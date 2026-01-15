# TurtleOpenGL — README

A short reference for the minimal Turtle Graphics-style example in `TurtleOpenGL.cpp`.

## Overview
This program demonstrates a tiny turtle graphics API implemented using legacy OpenGL + GLUT immediate-mode calls. It is educational code intended to show how simple turtle primitives map to drawing calls.

Location: `Topic1LabQuestion2/TurtleOpenGL.cpp`

## Build (macOS)

Compile with the system frameworks (Apple GLUT/OpenGL):

```bash
g++ TurtleOpenGL.cpp -o turtle -framework OpenGL -framework GLUT
```

(If you prefer absolute paths from repository root, run the command from `Topic1LabQuestion2`.)

## Run

From the `Topic1LabQuestion2` directory:

```bash
./turtle
```

A GLUT window will open and `TurtleDemoDrawing()` will execute, showing a few example shapes.

## Public turtle API (functions)

- `struct Turtle` — internal state:
  - `x, y` — current position in projection space
  - `angleDeg` — heading angle in degrees (0 = +X, increases CCW)
  - `penDown` — whether movement draws a line
  - `r, g, b` — current drawing color (0..1)

- `void TurtleSetPosition(float x, float y)`
  - Set the turtle's absolute position without drawing. Equivalent to lifting the pen and moving.

- `void TurtleRotateLeft(float angleDeg)`
  - Rotate the turtle counter-clockwise by `angleDeg` degrees. Angle is normalized into [0, 360).

- `void TurtleRotateRight(float angleDeg)`
  - Rotate the turtle clockwise by `angleDeg` degrees (implemented as negative rotation). Angle normalized.

- `void TurtlePenUp()` / `void TurtlePenDown()`
  - Toggle drawing: when the pen is down, subsequent moves produce line segments.

- `void TurtleSetColor(float r, float g, float b)`
  - Set the current color used for subsequent drawing (components in the 0..1 range).

- `void TurtleMoveForward(float distance)`
  - Move forward by `distance` along the current heading. If `penDown` is true, draw a line from the previous position to the new position.
  - Degrees are converted to radians internally using a `PI` constant for the trig functions.

- `void TurtleMoveTo(float x, float y, bool draw = true)`
  - New helper: move to the absolute coordinates `(x,y)`. If `draw==true`, draw a line connecting the current position to `(x,y)`; otherwise just move.

- `void TurtleDemoDrawing()`
  - Example code that resets the turtle state and draws a square, a triangle, and a small fan of rays. Useful as a usage example.

## Notes and caveats

- This example uses OpenGL's immediate-mode functions (`glBegin`/`glVertex`/`glEnd`). On modern macOS the OpenGL fixed-function API is deprecated; the example still runs but will emit deprecation warnings. For production or high-performance code, use modern OpenGL (shaders + buffer objects) or a higher-level cross-platform rendering library.

- Coordinates and distances are in the current projection space (the default `ReshapeCallback` sets a simple orthographic projection in the range -1..1 in at least one axis).

- Angle normalization is applied after rotations for easier debugging, but `sin`/`cos` will work with any angle.

- If you plan to add many segments or interactive features, consider storing drawn line segments in a container and re-rendering them in the display callback instead of issuing immediate draws inside each turtle function.

## Suggested small improvements

- Add `TurtleSetLineWidth(float width)` to control stroke thickness (call `glLineWidth`).
- Maintain a vector/list of segments for persistent rendering and easier features (undo/clear/save).
- Provide a simple interactive input loop (keyboard) to drive the turtle in real time.

---

If you want, I can: (a) update `TurtleDemoDrawing()` to use `TurtleMoveTo()` in places where absolute moves occur, (b) add a `TurtleSetLineWidth()` helper, or (c) implement a persistent line-list for re-rendering. Tell me which you'd like next.
