# Dusk Game/Simulation Engine

[![CircleCI](https://circleci.com/gh/WhoBrokeTheBuild/Dusk.svg?style=svg)](https://circleci.com/gh/WhoBrokeTheBuild/Dusk)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/WhoBrokeTheBuild/Dusk?svg=true)](https://ci.appveyor.com/project/WhoBrokeTheBuild/dusk)

## Building

```sh
# Configure
mkdir Build
cd Build
cmake ..

# Build
make -l
# or
cmake --build .
```

Specify build options to CMake before building.

```sh
cd Build
cmake -DOPTION=VALUE ..
```

### Available Build Options

> `VERBOSE_LOGGING`, default `OFF`
>
> Enables verbose logging with the `DuskLogVerbose` function.

> `BUILD_TESTS`, default `OFF`
>
> Build tests, which can then be run with `ctest` or `make test`.

> `BUILD_DEMOS`, default `ON`
>
> Build demo projects, which can be run with `make run-DemoName`.

> `BUILD_THIRDPARTY`, default `ON`
>
> Build any missing third-party dependencies.

> `BUILD_PACKAGES`, default `OFF`
>
> Build packages for distribution (`.deb`, `.rpm`, `.tgz`, etc.).

> `MODULES`, defaults to modules required by demos
>
> A semicolon-separated list of modules to build. Wrap in "" in order to keep
> it from being misinterpreted by the shell. Run CMake to display a list of
> available modules.
>
> e.g. `cmake -DMODULES="OpenGL;Vulkan" ..`

## Running Examples

The examples are built to run from their source directories. To facilitate this, there are convenience targets prefixed with `run-`

```
make -l run-HelloWorld
# or
cmake --build . --target run-HelloWorld
```

## Contributing

### Pull Requests

Use the "Branch and Pull Request" model for contributing changes.
