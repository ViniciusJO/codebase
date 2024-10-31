# C codebase

Inspired by:
- [hutusi/rethink-c](https://github.com/hutusi/rethink-c)
- [fragglet/c-algorithms](https://github.com/fragglet/c-algorithms)
- [PixelRifts/c-codebase](https://github.com/PixelRifts/c-codebase)
- [nothings/stb](https://github.com/nothings/stb)

## Usage

For the most parts, try to implemet header only libraries in a [stb](https://github.com/nothings/stb) style. So on using `example_lib.h` do, wherever the implementation is intended to be:

```c
// ...

#define EXAMPLE_LIB_IMPLEMENTATION
#include "example_lib.h"

// ...
```

Make shure to define `EXAMPLE_LIB_IMPLEMENTATION` in only one translation unit to avoid linking issues with multiple implementations.

## implementations

- [arraylist](src/arraylist.h)
