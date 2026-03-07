# Commands Examples

## Projections

#### Orthographic

```bash
make run ARGS="--rotate-y --orthographic --lines"
```

#### Perspective

1. Huge Perspective

```bash
make run ARGS="--rotate-y --perspective --lines --zc=1 --zsp=0.25"
```

2. Small Perspective (almost orthographic)

```bash
make run ARGS="--rotate-y --perspective --lines --zc=20 --zsp=0.25"
```

#### Axonometric

```bash
make run ARGS="--rotate-y --axonometric --lines --alpha=50 --beta=50"
```



## Lighting

#### Phong Lighting Model — Formula
```
I_out = Ia*Ka + I*(Kd*cosθ + Ks*cos^p α)
```

| Parameter | Range | Description |
|-----------|-------|-------------|
| `Ia` | 0..1 | ambient light intensity |
| `Ka` | 0..1 | material ambient reflection coefficient |
| `I`  | 0..1 | directed light source intensity |
| `Kd` | 0..1 | material diffuse reflection coefficient |
| `Ks` | 0..1 | material specular reflection coefficient |
| `p`  | 1..200 | shininess / polishing quality (higher = tighter highlight) |

---

#### 1. Ambient only — `Ia*Ka`

> Flat uniform lighting, no shading, unaffected by rotation

```bash
# full ambient
make run ARGS="--triangles --default --perspective --Ia=1 --Ka=1 --I=0"

# weak ambient light (Ia)
make run ARGS="--triangles --default --perspective --Ia=0.3 --Ka=1 --I=0"

# weak ambient material (Ka)
make run ARGS="--triangles --default --perspective --Ia=1 --Ka=0.1 --I=0"
```

---

#### 2. Diffuse only — `I*Kd*cosθ`

> Brightness depends on angle between face normal and light direction

```bash
# full diffuse
make run ARGS="--triangles --default --perspective --Ia=0 --I=1 --Kd=1 --Ks=0"

# weak light source (I)
make run ARGS="--triangles --default --perspective --Ia=0 --I=0.5 --Kd=1 --Ks=0"

# weak material (Kd)
make run ARGS="--triangles --default --perspective --Ia=0 --I=1 --Kd=0.3 --Ks=0"
```

---

#### 3. Specular only — `I*Ks*cos^p α`

> White highlight, depends on angle between reflected ray and viewer

```bash
# medium highlight
make run ARGS="--triangles --default --perspective --Ia=0 --I=1 --Kd=0 --Ks=1 --p=32"

# wide soft highlight (low p)
make run ARGS="--triangles --default --perspective --Ia=0 --I=1 --Kd=0 --Ks=1 --p=4"

# tight sharp highlight (high p)
make run ARGS="--triangles --default --perspective --Ia=0 --I=1 --Kd=0 --Ks=1 --p=200"
```

---

#### 4. Light source intensity — `I`

> Scales both diffuse and specular components together

```bash
# dim light
make run ARGS="--triangles --default --perspective --I=0.2 --Ks=0.5 --p=32"

# medium
make run ARGS="--triangles --default --perspective --I=0.5 --Ks=0.5 --p=32"

# bright
make run ARGS="--triangles --default --perspective --I=1.0 --Ks=0.5 --p=32"
```

---

#### 5. Full Phong model

```bash
make run ARGS="--triangles --default --perspective --Ia=1 --Ka=0.15 --I=1 --Kd=0.85 --Ks=0.5 --p=32"
```

---

#### 6. Material presets

```bash
# matte rubber — weak wide highlight
make run ARGS="--triangles --default --perspective --Ka=0.1 --Kd=0.9 --Ks=0.1 --p=4"

# metal — strong tight highlight
make run ARGS="--triangles --default --perspective --Ka=0.1 --Kd=0.6 --Ks=0.8 --p=128"

# plastic — medium highlight
make run ARGS="--triangles --default --perspective --Ka=0.2 --Kd=0.7 --Ks=0.5 --p=64"

# mirror — almost all specular, minimal diffuse
make run ARGS="--triangles --default --perspective --Ka=0.3 --Kd=0.3 --Ks=1.0 --p=200"
```

---

#### 7. Light direction — `lx, ly, lz`

```bash
# front (default)
make run ARGS="--triangles --rotate-y --perspective --lx=0 --ly=0 --lz=1"

# from the right
make run ARGS="--triangles --rotate-y --perspective --lx=1 --ly=0 --lz=0"

# from the left
make run ARGS="--triangles --rotate-y --perspective --lx=-1 --ly=0 --lz=0"

# from above
make run ARGS="--triangles --rotate-y --perspective --lx=0 --ly=1 --lz=0"

# from below
make run ARGS="--triangles --rotate-y --perspective --lx=0 --ly=-1 --lz=0"

# 3/4 light (classic portrait lighting)
make run ARGS="--triangles --rotate-y --perspective --lx=1 --ly=1 --lz=1"
```

---

#### 8. Projection comparison with full lighting

```bash
make run ARGS="--triangles --default --orthographic --Ia=1 --Ka=0.15 --I=1 --Kd=0.85 --Ks=0.5 --p=32"
make run ARGS="--triangles --default --perspective  --Ia=1 --Ka=0.15 --I=1 --Kd=0.85 --Ks=0.5 --p=32"
make run ARGS="--triangles --default --axonometric  --Ia=1 --Ka=0.15 --I=1 --Kd=0.85 --Ks=0.5 --p=32"
```
