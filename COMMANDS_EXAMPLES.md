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

