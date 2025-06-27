
---

## How to build

**Windows**：
- **way 1**：use Virtual Studio open sln/goofer.sln and test/test.sln
- **way 2**：use CMake to generate projects
  - `cmake -S . -B build -D BUILD_STATIC=OFF -D BUILD_TESTS=ON -G "Visual Studio 17 2022"`
  - `cmake --build build`

---

**Unix-Like**：
- **way 1**：`cd sln && make` to build goofer, `cd test && make` to build test, 
- **way 2**：use CMake to generate projects
  - `cmake -S . -B build -D BUILD_STATIC=OFF -D BUILD_TESTS=ON`
  - `cmake --build build`

---
