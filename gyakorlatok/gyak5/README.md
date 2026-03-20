# Graphics Practice 5 (gyak5) - Origin Graphics Engine

## Implementált Funkciók

### 🎮 Kamera Vezérlés (Új)
- **Q / E billentyűk**: Függőleges mozgás (felfelé / lefelé)
- **J / L billentyűk**: Kamera forgatása (balra / jobbra)
- **Egér drag**: Google Maps stílusú nézet forgatás (kattints és húzz)

### 🎨 Grafikai Objektumok (Új)

#### 1. Színes Háromszög
- Sárga, cián és magenta csúcsokkal
- Az egységkockán belül elhelyezve

#### 2. Tessellált Gömb
- 8×8-as tesszelláció (8 stack, 8 slice)
- Szürke szín
- Folyamatos forgás az átló tengelye körül (1°/frame sebességgel)
- Sugár: 0.15 egység

#### 3. Sakktábla Minta
- X-Y síkon (Z = 0)
- 5×5-ös minta
- Felváltva sötét és világos szürke

#### 4. Henger (Triangle Strips)
- 12 szelet, 3 stack
- Világoskék szín
- Sugár: 0.1, magasság: 0.2 egység

#### 5. Kúp (Triangle Fan)
- 12 szelet
- Világospiros szín
- Sugár: 0.1, magasság: 0.2 egység

## Fordítás és Futtatás

```bash
make
./origin.exe
```
