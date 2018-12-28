# Dusk Game/Simulation Engine

[![Build Status](https://travis-ci.org/WhoBrokeTheBuild/Dusk.svg?branch=master)](https://travis-ci.org/WhoBrokeTheBuild/Dusk)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/WhoBrokeTheBuild/Dusk?svg=true)](https://ci.appveyor.com/project/WhoBrokeTheBuild/dusk)

## Building

```
mkdir build && cd build
cmake ..
cmake --build .
```

## Running Examples

The examples are built to run from their source directories. To facilitate this, there are convenience targets prefixed with `run-`.

Example:
```
cmake --build . --target run-HelloWorld
```
