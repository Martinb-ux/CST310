# Sierpinski Gasket Project

## Project Description

This project implements three different versions of the **Sierpinski Gasket**, a classic fractal figure that serves as the "Hello World" of computer graphics. The Sierpinski Gasket demonstrates fundamental concepts in recursive algorithms, geometric transformations, and OpenGL rendering techniques.

The three implementations showcase different approaches to generating this fractal:
1. **2D Random Point Method** - Uses the chaos game algorithm with random point generation
2. **2D Subdivision Method** - Employs recursive triangle subdivision
3. **3D Tetrahedron Method** - Extends the concept to three dimensions using a tetrahedron

Each implementation allows for real-time parameter adjustments, enabling experimentation with the fractal's dimensional characteristics and visual properties.

---

## Methodology and Approach

### Overview
The project uses **OpenGL** (via GLUT/FreeGLUT) for graphics rendering and **C++** for implementation. Each version demonstrates a different algorithmic approach to generating fractals:

### 1. Random Point Method (Chaos Game)
**Approach**: Stochastic/probabilistic method
- Start with three vertices defining a triangle
- Begin at an arbitrary point
- Iteratively select a random vertex and plot the midpoint between the current position and that vertex
- The current position becomes the new midpoint
- After thousands of iterations, the Sierpinski pattern emerges

**Key Insight**: Random processes can generate deterministic fractal patterns through simple rules.

### 2. Subdivision Method
**Approach**: Deterministic recursive method
- Start with a single triangle
- Recursively subdivide each triangle into four smaller triangles by connecting edge midpoints
- Draw only the three outer triangles, omitting the center triangle
- Each recursion level increases the pattern detail

**Key Insight**: Fractals can be generated through systematic geometric subdivision.

### 3. 3D Tetrahedron Method
**Approach**: Extension to three dimensions
- Start with a regular tetrahedron (4 vertices, 4 triangular faces)
- Recursively subdivide into four smaller tetrahedra at each corner
- Apply 3D transformations (rotation, perspective projection)
- Uses depth buffering for proper 3D rendering

**Key Insight**: Fractal concepts extend naturally to higher dimensions.

---

## Algorithm Details

### Algorithm 1: Random Point Method (Chaos Game)

#### Pseudocode:
```
INITIALIZE:
    vertices[3] = {v1, v2, v3}  // Triangle vertices
    currentPoint = arbitrary point
    numIterations = 50000

FOR i = 0 TO numIterations:
    randomVertex = random(0, 2)
    currentPoint = midpoint(currentPoint, vertices[randomVertex])
    PLOT(currentPoint)
END FOR
```

#### Flowchart:
```
┌─────────────────────┐
│   Start Program     │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Define 3 vertices   │
│ of triangle         │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Initialize current  │
│ point at center     │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Set iteration count │
└──────────┬──────────┘
           │
           ▼
      ┌────────┐
      │ Loop:  │◄──────────┐
      │ i < N  │           │
      └────┬───┘           │
           │               │
           ▼               │
┌─────────────────────┐   │
│ Select random       │   │
│ vertex (0, 1, or 2) │   │
└──────────┬──────────┘   │
           │               │
           ▼               │
┌─────────────────────┐   │
│ Calculate midpoint  │   │
│ between current &   │   │
│ selected vertex     │   │
└──────────┬──────────┘   │
           │               │
           ▼               │
┌─────────────────────┐   │
│ Plot the point      │   │
└──────────┬──────────┘   │
           │               │
           ▼               │
┌─────────────────────┐   │
│ Update current      │   │
│ point = midpoint    │   │
└──────────┬──────────┘   │
           │               │
           ▼               │
┌─────────────────────┐   │
│ Increment i         │───┘
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│   Display Result    │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│   End Program       │
└─────────────────────┘
```

---

### Algorithm 2: Subdivision Method

#### Pseudocode:
```
FUNCTION subdivideTriangle(v1, v2, v3, depth):
    IF depth == 0:
        DRAW_TRIANGLE(v1, v2, v3)
    ELSE:
        // Calculate midpoints
        m12 = midpoint(v1, v2)
        m23 = midpoint(v2, v3)
        m31 = midpoint(v3, v1)
        
        // Recursively subdivide three outer triangles
        subdivideTriangle(v1, m12, m31, depth-1)
        subdivideTriangle(m12, v2, m23, depth-1)
        subdivideTriangle(m31, m23, v3, depth-1)
        // Note: Center triangle (m12, m23, m31) is NOT drawn
    END IF
END FUNCTION

MAIN:
    initialTriangle = {v1, v2, v3}
    depth = 5
    subdivideTriangle(initialTriangle, depth)
```

#### Flowchart:
```
┌────────────────────────┐
│ Start: subdivideTriangle│
│ (v1, v2, v3, depth)    │
└───────────┬────────────┘
            │
            ▼
       ┌─────────┐
       │depth==0?│
       └────┬────┘
            │
     ┌──────┴──────┐
     │             │
    YES            NO
     │             │
     ▼             ▼
┌─────────┐   ┌──────────────────┐
│  Draw   │   │ Calculate        │
│Triangle │   │ midpoints:       │
│         │   │ m12, m23, m31    │
└────┬────┘   └────────┬─────────┘
     │                 │
     │                 ▼
     │        ┌──────────────────┐
     │        │ Recurse on:      │
     │        │ Triangle(v1,m12,m31)│
     │        └────────┬─────────┘
     │                 │
     │                 ▼
     │        ┌──────────────────┐
     │        │ Recurse on:      │
     │        │ Triangle(m12,v2,m23)│
     │        └────────┬─────────┘
     │                 │
     │                 ▼
     │        ┌──────────────────┐
     │        │ Recurse on:      │
     │        │ Triangle(m31,m23,v3)│
     │        └────────┬─────────┘
     │                 │
     └────────┬────────┘
              │
              ▼
       ┌────────────┐
       │   Return   │
       └────────────┘
```

---

### Algorithm 3: 3D Tetrahedron Method

#### Pseudocode:
```
FUNCTION subdivideTetrahedron(v1, v2, v3, v4, depth):
    IF depth == 0:
        DRAW_TRIANGLE(v1, v2, v3)  // Face 1
        DRAW_TRIANGLE(v1, v3, v4)  // Face 2
        DRAW_TRIANGLE(v1, v4, v2)  // Face 3
        DRAW_TRIANGLE(v2, v4, v3)  // Face 4
    ELSE:
        // Calculate midpoints of all 6 edges
        m12 = midpoint(v1, v2)
        m13 = midpoint(v1, v3)
        m14 = midpoint(v1, v4)
        m23 = midpoint(v2, v3)
        m24 = midpoint(v2, v4)
        m34 = midpoint(v3, v4)
        
        // Recursively subdivide four corner tetrahedra
        subdivideTetrahedron(v1, m12, m13, m14, depth-1)
        subdivideTetrahedron(m12, v2, m23, m24, depth-1)
        subdivideTetrahedron(m13, m23, v3, m34, depth-1)
        subdivideTetrahedron(m14, m24, m34, v4, depth-1)
    END IF
END FUNCTION
```

---

## Key Code Explanations

### 1. Midpoint Calculation
The core operation in all three implementations is calculating midpoints:

```cpp
void calculateMidpoint(float p1[2], float p2[2], float result[2]) {
    result[0] = (p1[0] + p2[0]) / 2.0f;
    result[1] = (p1[1] + p2[1]) / 2.0f;
}
```

**Explanation**: This simple averaging operation is the mathematical foundation of the fractal generation. By repeatedly finding midpoints, we create self-similar patterns at different scales.

---

### 2. Random Vertex Selection (Chaos Game)
```cpp
// Randomly select one of the three vertices
int randomVertex = rand() % 3;

// Calculate midpoint between current point and selected vertex
calculateMidpoint(currentPoint, vertices[randomVertex], midpoint);

// Plot the midpoint
glVertex2fv(midpoint);

// Update current point
currentPoint[0] = midpoint[0];
currentPoint[1] = midpoint[1];
```

**Explanation**: The `rand() % 3` produces a random integer (0, 1, or 2), ensuring equal probability for each vertex. Despite the randomness, the algorithm converges to the Sierpinski pattern because the midpoint operation constrains points to specific regions.

---

### 3. Recursive Subdivision Logic
```cpp
void subdivideTriangle(point2 a, point2 b, point2 c, int depth) {
    if (depth == 0) {
        drawTriangle(a, b, c);  // Base case
    } else {
        // Calculate midpoints
        point2 ab, bc, ca;
        ab[0] = (a[0] + b[0]) / 2.0f;
        ab[1] = (a[1] + b[1]) / 2.0f;
        // ... (similar for bc and ca)
        
        // Recursively subdivide three outer triangles
        subdivideTriangle(a, ab, ca, depth - 1);   // Bottom-left
        subdivideTriangle(ab, b, bc, depth - 1);   // Bottom-right
        subdivideTriangle(ca, bc, c, depth - 1);   // Top
        // Note: Center triangle is NOT subdivided!
    }
}
```

**Explanation**: The recursion terminates at `depth == 0`, where actual triangles are drawn. The critical insight is that we make three recursive calls instead of four—the center triangle is omitted, creating the characteristic "holes" in the gasket. Each level of recursion increases the pattern complexity exponentially.

---

### 4. 3D Transformations and Perspective
```cpp
// Set up perspective projection
gluPerspective(60.0, 1.0, 0.1, 100.0);

// Position camera
gluLookAt(0.0, 0.0, 3.0,   // Eye position
          0.0, 0.0, 0.0,   // Look at point
          0.0, 1.0, 0.0);  // Up vector

// Apply rotations
glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
```

**Explanation**: 
- `gluPerspective` creates a realistic perspective projection (objects farther away appear smaller)
- `gluLookAt` positions a virtual camera 3 units away from the origin, looking at the center
- `glRotatef` applies rotations around the X and Y axes, allowing us to view the 3D structure from different angles

---

### 5. Depth Testing for 3D
```cpp
// Enable depth testing
glEnable(GL_DEPTH_TEST);

// Clear both color and depth buffers
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

**Explanation**: Depth testing (Z-buffering) ensures that closer triangles are drawn in front of farther ones. Without this, triangles would be drawn in arbitrary order, creating visual artifacts where back faces appear in front of front faces.

---

### 6. Normal Calculation for Lighting
```cpp
void calculateNormal(point3 a, point3 b, point3 c, float normal[3]) {
    float v1[3], v2[3];
    
    // Calculate two edge vectors
    v1[0] = b[0] - a[0];  v1[1] = b[1] - a[1];  v1[2] = b[2] - a[2];
    v2[0] = c[0] - a[0];  v2[1] = c[1] - a[1];  v2[2] = c[2] - a[2];
    
    // Cross product to find perpendicular vector
    normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
    
    // Normalize to unit length
    float length = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    if (length > 0.0f) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
}
```

**Explanation**: Surface normals are vectors perpendicular to a surface, used for lighting calculations. The cross product of two edge vectors gives a perpendicular vector. Normalizing (dividing by length) ensures consistent lighting regardless of triangle size.

---

## Interesting Observations

### Self-Similarity Across Scales
Each zoomed-in portion of the Sierpinski Gasket looks identical to the whole—a defining property of fractals. This is particularly evident in the subdivision method where each recursion level creates three identical copies at half scale.

### Fractal Dimension
The Sierpinski Gasket has a **fractal dimension of approximately 1.585** (log(3)/log(2)), meaning it's more complex than a 1D line but less than a 2D plane. This fractional dimension reflects its intricate structure.

### Performance Considerations
```cpp
// Number of triangles = 3^depth
// For depth=5: 3^5 = 243 triangles
// For depth=8: 3^8 = 6,561 triangles
```
The exponential growth in triangle count makes high recursion depths computationally expensive. The random point method trades determinism for computational efficiency—thousands of points can be generated quickly.

### Chaos to Order
The random point method demonstrates an emergent property: order arising from randomness. Despite random vertex selections, the constraint of always choosing midpoints forces points into the fractal pattern.

---

## Interactive Controls

### 2D Random Point Method:
- `+/-`: Adjust number of points (affects detail)
- `R`: Regenerate with new random sequence
- `ESC`: Exit

### 2D Subdivision Method:
- `+/-`: Change subdivision depth (0-8)
- `SPACE`: Toggle between filled and wireframe modes
- `R`: Reset to defaults
- `ESC`: Exit

### 3D Tetrahedron Method:
- `+/-`: Adjust subdivision depth (0-6)
- `Arrow Keys`: Manual rotation
- `SPACE`: Toggle automatic rotation animation
- `W`: Toggle wireframe mode
- `R`: Reset rotation
- `ESC`: Exit

---

## Experimental Parameters

### Effect of Iteration Count (2D Random)
- **Low (< 5,000)**: Sparse, pattern barely visible
- **Medium (10,000-50,000)**: Pattern clearly emerges
- **High (> 100,000)**: Dense, smooth appearance

### Effect of Subdivision Depth (Both 2D & 3D)
- **Depth 0**: Just the initial shape
- **Depth 1-3**: Basic fractal structure visible
- **Depth 4-6**: Intricate detail becomes apparent
- **Depth 7+**: Maximum detail (performance intensive)

### Visual Modes
- **Filled Mode**: Solid triangles, emphasizes area and color
- **Wireframe Mode**: Shows only edges, reveals internal structure

---

## Mathematical Foundation

The Sierpinski Gasket can be described mathematically using **Iterated Function Systems (IFS)**:

Three affine transformations, each mapping to a half-scale copy:

$$
w_1(x, y) = \left(\frac{x}{2}, \frac{y}{2}\right)
$$

$$
w_2(x, y) = \left(\frac{x}{2} + \frac{1}{4}, \frac{y}{2} + \frac{\sqrt{3}}{4}\right)
$$

$$
w_3(x, y) = \left(\frac{x}{2} + \frac{1}{2}, \frac{y}{2}\right)
$$

The gasket is the unique attractor of this IFS, which is why the random method converges to it.

---

## Technical Notes

### Coordinate Systems
- **2D**: Uses orthographic projection with coordinates [-1, 1] for both x and y
- **3D**: Uses perspective projection with the camera positioned along the Z-axis

### Color Schemes
- **2D Random**: Green points on black background (high contrast)
- **2D Subdivision**: Gradient based on depth (visual depth indicator)
- **3D Tetrahedron**: Different colors for each face (red, green, blue, yellow)

### Performance Optimization
- Point-based rendering for random method (efficient for many points)
- Triangle batching in subdivision methods
- Depth testing only enabled when necessary (3D version)

---

## macOS OpenGL deprecation note

On recent versions of macOS the legacy OpenGL API is marked deprecated by Apple and the system headers may emit deprecation warnings when compiling. This repository intentionally uses legacy OpenGL/GLUT for simplicity and educational purposes.

Workaround included in this project:

- The `Makefile` defines `GL_SILENCE_DEPRECATION` in `CXXFLAGS` so the compiler will not print deprecation warnings when building on macOS.

If you want to remove the warnings manually instead, you can either:

- Define the macro on the compiler command-line: `-DGL_SILENCE_DEPRECATION`.
- Or set it inside source files before including OpenGL headers (not necessary here because the Makefile handles it).

Note: Silencing warnings does not change the fact that OpenGL is deprecated on macOS; for production or forward-looking code, consider porting rendering to Metal or using a cross-platform modern OpenGL setup with an abstraction layer.


## Future Enhancements

Possible extensions to explore:
1. **Texture mapping** on the fractal surfaces
2. **Lighting models** for more realistic 3D rendering
3. **Animation** of the subdivision process (step-by-step visualization)
4. **Other fractals**: Menger sponge, Koch snowflake, dragon curve
5. **Interactive point placement** for 2D version
6. **Export functionality** to save images or 3D models

---

## References

- Interactive Computer Graphics (7th Edition) - Chapter 2: Graphics Programming
- OpenGL Programming Guide (Red Book)
- "Fractals Everywhere" by Michael Barnsley
- Sierpiński, W. (1915). "Sur une courbe dont tout point est un point de ramification"

---

## Conclusion

This project demonstrates fundamental computer graphics concepts including:
- Recursive algorithms and their visual manifestation
- 2D and 3D geometric transformations
- OpenGL rendering pipeline (vertex specification, transformations, rasterization)
- Interactive real-time graphics
- Mathematical principles underlying fractal geometry

The Sierpinski Gasket serves as an excellent introduction to computer graphics because it combines simple rules with complex, beautiful output—a pattern that recurs throughout computer graphics and procedural generation.
