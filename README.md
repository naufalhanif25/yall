# YALL (Yet Another Low Level)

**YALL (Yet Another Low Level)** is a minimalist, assembly-inspired low-level programming language built around simple labels and instructions, designed for fast execution and explicit control flow.

## Features

- Assembly-like syntax with readable instructions
- Label-based program flow (`goto`, `goif`)
- Simple execution model (no classes, no scopes)
- Fast and lightweight runtime
- Easy to parse and extend

## Example

```
main:
    std.store(index, 0)
    std.store(start, time.now())
    std.goto(loop)

loop:
    std.store(index, math.add(std.load(index), 1))
    std.cout(std.load(index))
    std.goif(math.equ(std.load(index), 1000), exit)
    std.goto(loop)

exit:
    std.store(end, time.now())
    std.store(exec_time, time.mili(time.diff(std.load(start), std.load(end))))
    std.cout("\nExecuted in", std.load(exec_time), "ms")
    std.exit(0)
```

The following example demonstrates a simple loop in YALL that counts from 1 to 1000 and prints the execution time.

## Build, Run, and Test

### Build
Compile the YALL project using `make`:

```bash
make build
```

The build output will be generated in the `bin` directory.

### Run

After building, run the YALL executable:
```bash
./yall --run path/to/script.yall
```

Replace `script.yall` with your YALL source file.

### Test

Run all unit tests to ensure everything works correctly:

```bash
make test
```

## License

This project is distributed under the MIT License. See the `LICENSE` file for full details.