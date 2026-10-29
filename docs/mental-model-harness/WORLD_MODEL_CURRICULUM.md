# Grounded World Model: CO1035 / CO1301 Curriculum & Engine Reference

This document contains the verified, syllabus-compliant technical facts and mathematical equations for UCLan Cyprus module **CO1035 / CO1301 Games Concepts**.

---

## 1. Engine & 3D Space Invariants

### 1.1 Coordinate System
- **Handedness**: Left-handed 3D Cartesian coordinate system.
- **Axes**:
  - $+X$: East / Right
  - $+Y$: Up (Vertical elevation)
  - $+Z$: North / Forward (Depth into screen)
- **Rotation**:
  - In degrees.
  - Positive Y rotation rotates clockwise when viewed from above.

### 1.2 Pivot Points & Center Origins
- In TL-Engine meshes (`.x` files), the origin `(0, 0, 0)` is at the **geometric center (pivot)** of the object.
- **Vertical Alignment Formula**:
  $$\text{Model Placement } Y = \text{Ground Level } Y + \text{Half-Height (or Radius)}$$

---

## 2. Core Mathematics & Physics

### 2.1 3D Vectors Between Points
Vector $\vec{V}$ from point $P_1(x_1, y_1, z_1)$ to $P_2(x_2, y_2, z_2)$:
$$\vec{V} = (x_2 - x_1, \; y_2 - y_1, \; z_2 - z_1)$$

### 2.2 Vector Length & Squared Distance Optimization
Length (Euclidean Norm) using Pythagoras' Theorem:
$$\|\vec{V}\| = \sqrt{dx^2 + dy^2 + dz^2}$$

**Performance Optimization (Squared Distance)**:
Square root calculations are computationally expensive in game loops. Avoid `sqrt` by comparing squared values:
$$D^2 = dx^2 + dy^2 + dz^2 \le R^2$$

### 2.3 Vector Normalization (Unit Direction Vector)
To convert a direction vector into a unit vector of length 1:
$$\hat{V} = \frac{\vec{V}}{\|\vec{V}\|}$$

### 2.4 Collision Detection Types

#### A. Point-to-Sphere Collision
A point $P(x, y, z)$ collides with a sphere of center $S(x, y, z)$ and radius $R$ if:
$$\text{Distance}^2(P, S) \le R^2$$

#### B. Sphere-to-Sphere Collision
Two spheres $(S_1, R_1)$ and $(S_2, R_2)$ intersect if:
$$\text{Distance}^2(S_1, S_2) \le (R_1 + R_2)^2$$

#### C. Sphere-to-Box (AABB) Collision
Clamps the sphere center coordinates to the Axis-Aligned Bounding Box ranges:
```cpp
float Clamp(float val, float minVal, float maxVal) {
    if (val < minVal) return minVal;
    if (val > maxVal) return maxVal;
    return val;
}

bool SphereToBox(float sX, float sY, float sZ, float sR,
                 float bMinX, float bMaxX, float bMinY, float bMaxY, float bMinZ, float bMaxZ) {
    float cx = Clamp(sX, bMinX, bMaxX);
    float cy = Clamp(sY, bMinY, bMaxY);
    float cz = Clamp(sZ, bMinZ, bMaxZ);
    return DistanceSquared(sX, sY, sZ, cx, cy, cz) <= (sR * sR);
}
```

---

## 3. Frame-Rate Independent Delta Timing

To ensure physics and movement run identically across all hardware:
$$\Delta\text{Position} = \text{Speed} \times \text{FrameTime}$$
```cpp
float frameTime = myEngine->FrameTime();
player->MoveLocalZ(kSphereSpeed * frameTime);
```

---

## 4. Modern TL-Engine 11 vs. Legacy Lab Translation

| Concept | Modern TL-Engine 11 | Legacy Lab Type |
| :--- | :--- | :--- |
| **Engine Pointer** | `TLEngine* myEngine` | `I3DEngine* myEngine` |
| **Mesh Blueprint** | `Mesh* mesh` | `IMesh* mesh` |
| **Model Instance** | `Model* model` | `IModel* model` |
| **Camera** | `Camera* camera` | `ICamera* camera` |
| **Engine Creation** | `New3DEngine(TL11)` | `New3DEngine(kTLX)` |
| **Camera Types** | `ManualCamera`, `FPSCamera` | `kManual`, `kFPS` |
| **Key Codes** | `Key_W`, `Key_Escape` | `EKeyCode::Key_W` |
