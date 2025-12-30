# Mathematical Foundations

## Vector Operations

### Vec3

3D vector $\mathbf{v} = (x, y, z)$ with operations:

- **Dot product**: $\mathbf{a} \cdot \mathbf{b} = a_x b_x + a_y b_y + a_z b_z$
- **Cross product**: $\mathbf{a} \times \mathbf{b} = (a_y b_z - a_z b_y, a_z b_x - a_x b_z, a_x b_y - a_y b_x)$
- **Magnitude**: $|\mathbf{v}| = \sqrt{x^2 + y^2 + z^2}$
- **Normalization**: $\hat{\mathbf{v}} = \frac{\mathbf{v}}{|\mathbf{v}|}$

## Matrix Operations

### Rotation Matrices

Rotation around axis by angle $\theta$:

**X-axis**:
$$R_x(\theta) = \begin{bmatrix}
1 & 0 & 0 \\
0 & \cos\theta & -\sin\theta \\
0 & \sin\theta & \cos\theta
\end{bmatrix}$$

**Y-axis**:
$$R_y(\theta) = \begin{bmatrix}
\cos\theta & 0 & \sin\theta \\
0 & 1 & 0 \\
-\sin\theta & 0 & \cos\theta
\end{bmatrix}$$

**Z-axis**:
$$R_z(\theta) = \begin{bmatrix}
\cos\theta & -\sin\theta & 0 \\
\sin\theta & \cos\theta & 0 \\
0 & 0 & 1
\end{bmatrix}$$

Composite rotation: $R = R_x(\alpha) R_y(\beta) R_z(\gamma)$

## Perspective Projection

Project 3D point $\mathbf{p} = (x, y, z)$ to screen coordinates:

$$x_{screen} = \frac{x}{z + f} \cdot w \cdot s + \frac{w}{2}$$

$$y_{screen} = \frac{-y}{z + f} \cdot h \cdot s + \frac{h}{2}$$

Where:
- $f$ = field of view distance
- $w, h$ = screen dimensions
- $s$ = scale factor

Depth preserved: $z_{screen} = z$

## Lighting

### Lambertian Shading

Intensity at vertex:
$$I = I_a + I_d \max(0, \mathbf{n} \cdot \mathbf{l})$$

Where:
- $I_a$ = ambient intensity
- $I_d$ = diffuse intensity
- $\mathbf{n}$ = normalized surface normal
- $\mathbf{l}$ = normalized light direction

### Gouraud Interpolation

Per-pixel intensity via barycentric coordinates:
$$I_p = w_0 I_0 + w_1 I_1 + w_2 I_2$$

Where $w_0 + w_1 + w_2 = 1$ and $w_i \geq 0$

## Barycentric Coordinates

For point $\mathbf{p}$ in triangle with vertices $\mathbf{v}_0, \mathbf{v}_1, \mathbf{v}_2$:

$$\mathbf{p} = w_0 \mathbf{v}_0 + w_1 \mathbf{v}_1 + w_2 \mathbf{v}_2$$

Using edge vectors:
- $\mathbf{e}_0 = \mathbf{v}_1 - \mathbf{v}_0$
- $\mathbf{e}_1 = \mathbf{v}_2 - \mathbf{v}_0$
- $\mathbf{e}_2 = \mathbf{p} - \mathbf{v}_0$

$$w_1 = \frac{\mathbf{e}_2 \times \mathbf{e}_1}{\mathbf{e}_0 \times \mathbf{e}_1}, \quad w_2 = \frac{\mathbf{e}_0 \times \mathbf{e}_2}{\mathbf{e}_0 \times \mathbf{e}_1}, \quad w_0 = 1 - w_1 - w_2$$

## Backface Culling

Reject triangle if:
$$\mathbf{n}_{face} \cdot \mathbf{v}_{view} > 0$$

Where $\mathbf{v}_{view} = (0, 0, -1)$ (camera direction)

