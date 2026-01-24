# A Minecraft savefile homology calculator

A program that computes homology of a space defined in a Minecraft
save file.

---

## Installation

In order to install the program, first make sure to have zlib library installed.
Next, type in the following commands:

```bash
git clone https://github.com/JKural/mc-homology.git
cmake --preset release
cmake --build build
cmake --install build [--prefix <install-prefix>]
```

`--prefix <install-prefix>` can be used to select a different base directory
for installation.

## Usage

```bash
mc-homology [-h | --help] [--Z | --Z2 | --Z3] [--latex | --no-latex] \
  [--x <x1> <x2>] [--y <y1> <y2>] [--z <z1> <z2>] <path-to-region-directory>
```

### Options

- `-h | --help`  
  Print help and exit.
- `--Z | --Z2 | --Z3`  
  Choose coefficients of the chain complexes.
  - `Z` - Integers
  - `Z2` - Integers mod 2
  - `Z3` - Integers mod 3
- `--latex | --no-latex`  
  Choose, whether to print the output in the form of a
  .tex file.
- `--x <x1> <x2>`  
  Choose x bounds of the save file used in calculations. The bounds are
  left inclusive and right exclusive.
- `--y <y1> <y2>`  
  Choose y bounds of the save file used in calculations. The bounds are
  left inclusive and right exclusive.
- `--z <z1> <z2>`  
  Choose x bounds of the save file used in calculations. The bounds are
  left inclusive and right exclusive.
- `<path-to-region-directory>`  
  Path to the region directory of the save file.
  Usually `.minecraft/saves/<Save name>/region`,
  `.minecraft/saves/<Save name>/DIM1/region` or
  `.minecraft/saves/<Save name>/DIM-1/region`.
