# bb (ByteBuilder)

A simple C++ tool for generating files of any size with custom content. Perfect for testing storage, benchmarking, or creating dummy data.

## Features

- Create files with sizes expressed in human-readable units such as B, KiB, MiB, GiB, and TiB
- Optionally create sparse files for efficient large file generation
- Verify files against an expected size and pattern

## Requirements

- Nix for the provided flake-based development shell
   If nix isn't available to your disposal:
      - A C++20-compatible compiler such as Clang, GCC, or MSVC
      - Meson and Ninja

## Build

From the repository root:

```bash
meson setup builddir
meson compile -C builddir
```

## Install

Install the binary to your configured prefix:

```bash
meson install -C builddir
```

For a userlocal installation that is available on your PATH:

```bash
meson setup builddir --prefix="$HOME/.local"
meson compile -C builddir
meson install -C builddir
export PATH="$HOME/.local/bin:$PATH"
```

## Usage

### Create a file

```bash
./builddir/bb create --output /tmp/example.bin --size 1MiB --pattern zero

# or

bb create --output /tmp/example.bin --size 1MiB --pattern zero
```

### Create a sparse file

```bash
./builddir/bb create --output /tmp/example.bin --size 1GiB --pattern zero --sparse

# or

bb create --output /tmp/example.bin --size 1GiB --pattern zero --sparse
```

### Verify a file

```bash
./builddir/bb verify --output /tmp/example.bin --size 1MiB --pattern zero

# or

bb verify --output /tmp/example.bin --size 1MiB --pattern zero
```

### Show help

```bash
./builddir/bb --help
./builddir/bb create --help
./builddir/bb verify --help

# or

bb --help
bb create --help
bb verify --help
```

## Supported options

- `--output <path>`: destination or source file path
- `--size <size>`: file size such as `1MiB`, `512K`, or `2G`
- `--pattern <name>`: `zero`, `incrementing`, or `random`
- `--seed <value>`: seed for random/incrementing generation
- `--sparse`: create a sparse file when possible
- `--no-progress`: disable progress output
- `--verify`: verify immediately after creating the file

## Development shell with Nix

If you prefer a reproducible dev environment:

```bash
nix develop
```