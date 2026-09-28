# bb (ByteBuilder)

A simple C++ tool for generating files of any size with custom content. Perfect for testing storage, benchmarking, or creating dummy data.

## Features

- Create files with decimal (KB, MB, GB, TB, PB) or binary (KiB, MiB, GiB, TiB, PiB) units
- Optionally create sparse files for efficient large file generation
- Verify files against an expected size and pattern
- Inspect exact file sizes in bytes, bits, decimal units, and binary units

## Requirements

- Nix for the provided flake-based development shell
- If Nix isn't available:
  - A C++20-compatible compiler such as Clang, GCC, or MSVC
  - Nox

## Build

From the repository root:

```bash
nox setup builddir
nox compile -C builddir
```

## Install

Install the binary to your configured prefix:

```bash
nox install -C builddir
```

For a userlocal installation that is available on your PATH:

```bash
nox setup builddir
nox compile -C builddir
nox install -C builddir --prefix="$HOME/.local"
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

### Inspect a file size

```bash
bb size /tmp/example.bin
```

The report includes the exact byte and bit counts, followed by exact decimal and binary unit conversions.
`KB` through `PB` use powers of 1000; `KiB` through `PiB` use powers of 1024.

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
- `--size <size>`: file size such as `500MB`, `1MiB`, `512K`, or `2G`
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