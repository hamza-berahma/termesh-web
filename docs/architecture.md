# Architecture

## Pipeline Overview

```mermaid
graph TD
    A[STL File] --> B[Model Loader]
    B --> C[Triangle Mesh]
    C --> D[Model Transform]
    D --> E[Backface Culling]
    E --> F[Perspective Projection]
    F --> G[Lighting Calculation]
    G --> H[Barycentric Rasterization]
    H --> I[Z-Buffer Test]
    I --> J[ASCII Buffer]
    J --> K[Output]
    
    D --> D1[Rotation Matrix]
    G --> G1[Lambertian Shading]
    H --> H1[Gouraud Interpolation]
```

## Data Flow

1. **Model Loading**: STL → `Triangle` mesh
2. **Transform**: Apply rotation matrix $R$ to vertices and normals
3. **Culling**: Reject back-facing triangles via dot product
4. **Projection**: 3D → 2D using perspective transform
5. **Lighting**: Per-vertex intensity via $\mathbf{n} \cdot \mathbf{l}$
6. **Rasterization**: Barycentric interpolation with z-buffer
7. **Output**: Character buffer → terminal/WASM

## Module Dependencies

```
math3d (no deps)
  ↓
model, projection, lighting, rasterizer
  ↓
renderer
```

