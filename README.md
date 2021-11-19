ShiftIntrinsicGuide
=============
[![Github All Releases](https://img.shields.io/github/downloads/Sibras/ShiftIntrinsicGuide/total.svg)](https://github.com/Sibras/ShiftIntrinsicGuide/releases)
[![GitHub release](https://img.shields.io/github/release/Sibras/ShiftIntrinsicGuide.svg)](https://github.com/Sibras/ShiftIntrinsicGuide/releases/latest)
[![GitHub issues](https://img.shields.io/github/issues/Sibras/ShiftIntrinsicGuide.svg)](https://github.com/Sibras/ShiftIntrinsicGuide/issues)
[![license](https://img.shields.io/github/license/Sibras/ShiftIntrinsicGuide.svg)](https://github.com/Sibras/ShiftIntrinsicGuide)
## ShiftIntrinsicGuide

ShiftIntrinsicGuide is a GUI for x86 intrinsic instruction information. It proveds a searchable list of all currently available intrinsic instructions and combines it with real-world latency/throughput measurements from a variet of different hardware architectures (Intel and AMD).

It combines data from the following sources:

- Intrinsics from [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html).
- Performance data from [uops.info](https://www.uops.info).

## Screenshot

![Screenshot](https://sibras.github.io/ShiftIntrinsicGuide/Screenshot.jpg "Example program screenshot")

## Features

- Searchable list of all available x86 intrinsic instructions.
- Instructions searchable by:
    - Intrinsic name
    - Assembly instruction
    - Required x86 technology level (e.g. SSE3, AVX etc.)
    - Types operated on (e.g. float, 8-bit integers etc.)
    - Instruction category (e.g. arithmetic, bitwise etc.)
- Displays information for each intrinsic such as:
    - Intrinsic form
    - Assembly instruction
    - Required include header
    - Required CPUID support
    - Instruction description
    - Instruction operation pseudo code
- Displays performance measurements such as:
    - Latency
    - Throughput
    - uops
    - Port usage
- Offline use: Data is directly scraped from the online sources on first run and cached for future use.
- Periodic data update checks (as well as manual)

## Downloads

Pre-built executables are available from the [releases](https://github.com/Sibras/ShiftIntrinsicGuide/releases) page in a single archive.

## Issues

Any issues related to ShiftIntrinsicGuide should be sent to the [issues](https://github.com/Sibras/ShiftIntrinsicGuide/issues) page for the repository.

## License

ShiftMediaProject original code is released under [Apache 2.0](https://opensource.org/licenses/Apache-2.0).

## Copyright

For a complete copyright list please checkout the source code to examine license headers. Unless expressly stated otherwise all code submitted to ShiftIntrinsicGuide (in any form) is licensed under [Apache 2.0](https://opensource.org/licenses/Apache-2.0) and copyright is donated to ShiftIntrinsicGuide. If you submit code that is not your own work it is your responsibility to place a header stating the copyright.

## Contributing

Patches related to ShiftIntrinsicGuide should be sent as pull requests to the main repository.