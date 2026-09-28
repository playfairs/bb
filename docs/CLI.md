# Command-Line Reference

## Create

Start interactive creation:

```bash
bb create
```

Enter a positive whole-number size, then select a unit from `KB`, `MB`, `GB`, `TB`, `PB`, `KiB`, `MiB`, `GiB`, `TiB`, or `PiB`.

Non-interactive creation accepts a combined size:

```bash
bb create --output example.bin --size 500MB --pattern zero
```

## Verify

Check a file against an expected size and content pattern:

```bash
bb verify --output example.bin --size 500MB --pattern zero
```

## Size

Show the exact file size and conversions:

```bash
bb size example.bin
```

The report lists the exact byte and bit counts, followed by exact values in decimal SI units and binary IEC units. Decimal units use powers of 1000 (`1 MB = 1,000,000 bytes`); binary units use powers of 1024 (`1 MiB = 1,048,576 bytes`).

## Unit Suffixes

- `KB`, `MB`, `GB`, `TB`, and `PB` are decimal units.
- `KiB`, `MiB`, `GiB`, `TiB`, and `PiB` are binary units.
- Legacy single-letter suffixes `K`, `M`, `G`, `T`, and `P` remain binary.
- `B` or no suffix means bytes; lowercase `b` means bits.
- Bit suffixes such as `kb`, `Mb`, and `Kib` are accepted when they represent a whole number of bytes.
