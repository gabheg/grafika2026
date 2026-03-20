# OBJ Fájlformátum

Az OBJ (Wavefront OBJ) egy szövegalapú 3D modell fájlformátum.

## Fájlstruktúra

### Vertexek (v)
- `v x y z [w]` - 3D pont koordináták (w: homogén koordináta, opcionális, alapértelmezett: 1.0)
- Például: `v 0.5 0.5 -0.5` - egy pont a (0.5, 0.5, -0.5) koordinátán

### Textúrakoordinták (vt)
- `vt u v [w]` - 2D (vagy 3D) textúrakoordináták (w: opcionális)
- Például: `vt 0.0 0.0` - textúra bal-alsó sarka
- Például: `vt 1.0 1.0` - textúra jobb-felső sarka

### Normálvektorok (vn)
- `vn nx ny nz` - 3D normálvektor (a fény számításához szükséges)
- Például: `vn 0.0 1.0 0.0` - fölfele mutató normál

### Arcok/Lapok (f)
- `f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3` - Háromszög (3 pont)
- Az index 1-től kezdődik (nem 0-tól)!
- Lehet 4 vagy több pont (poligon)
- Például: `f 1/1/1 2/2/1 3/3/1` - egy háromszög az 1., 2., 3. vertexből

## Példa - Kocka

```
# 8 vertex (sarok pontok)
v -0.5 -0.5  0.5
v  0.5 -0.5  0.5
v -0.5  0.5  0.5
v  0.5  0.5  0.5
v -0.5  0.5 -0.5
v  0.5  0.5 -0.5
v -0.5 -0.5 -0.5
v  0.5 -0.5 -0.5

# 4 textúrakoordináta (ugyanez ismétlődik az összes lapon)
vt 0.0 0.0
vt 1.0 0.0
vt 0.0 1.0
vt 1.0 1.0

# 6 normálvektor (minden oldalhoz egy)
vn  0.0  0.0  1.0  # elöl
vn  0.0  1.0  0.0  # fel
vn  0.0  0.0 -1.0  # hátul
vn  0.0 -1.0  0.0  # le
vn  1.0  0.0  0.0  # jobb
vn -1.0  0.0  0.0  # bal

# 12 háromszög (2 per lap, 6 lap)
f 1/1/1 2/2/1 3/3/1
f 3/3/1 2/2/1 4/4/1
...
```

## A cube.obj és cat.obj szerkezete

- **cube.obj**: 8 vertex, 4 textúra, 6 normál, 12 háromszög
- **cat.obj**: Bonyolultabb modell, több vertex (macska fejét ábrázolja)

## Adatok tárolása a kódban

A `libobj` könyvtár (obj/model.h) a Model struktúrában tárolja:
- `n_vertices`: csúcspontok száma
- `n_texture_vertices`: textúrakoordináták száma
- `n_normals`: normálvektorok száma
- `n_triangles`: háromszögek száma
- `vertices[]`: 3D pontok tömbjei
- `texture_vertices[]`: 2D textúra koordináták
- `normals[]`: normálvektorok
- `triangles[]`: háromszögek (3 pont indexek/textúra/normál combo)
