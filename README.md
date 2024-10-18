
# Dusk

The latest in a long line of 3D Game Engine attempts

## Building

```
mkdir build
cd build

cmake [-DGLTF_SAMPLE_ASSETS_PATH=path/to/glTF-Sample-Assets/] ..
cmake --build .
```

## Running

```
# with -DCMAKE_BUILD_TYPE=Debug
./build/Debug/HelloWorld

# with -DCMAKE_BUILD_TYPE=Release
./build/Release/HelloWorld
```