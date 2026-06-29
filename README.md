# Computer Vision

C++/OpenCV coursework built with CMake and vcpkg. `russian_plate` is the
primary application. Every homework assignment is also a standalone executable.

## CLion setup

1. Install [vcpkg](https://learn.microsoft.com/vcpkg/get_started/get-started)
   and set `VCPKG_ROOT` to its installation directory.
2. Restart CLion and open this repository's root directory.
3. Pick a CMake preset:
   - Visual Studio/MSVC toolchain: `windows-msvc-debug`
   - MinGW toolchain: `windows-mingw-debug`
4. Select an executable, such as `russian_plate` or `exercise_2_1`, from the
   run-target dropdown and run it.

*Note: the first time configuring the project takes a while.*

## Command line

Configure and build everything:

```sh
cmake --preset windows-msvc-debug
cmake --build --preset windows-msvc-debug
```

If you are using MinGW instead of MSVC:

```sh
cmake --preset windows-mingw-debug
cmake --build --preset windows-mingw-debug
```

Build one assignment:

```sh
cmake --build --preset windows-msvc-debug --target exercise_2_1
```

Programs are placed in the selected preset's `bin` directory, such as
`cmake-build/windows-msvc-debug/bin` or `cmake-build/windows-mingw-debug/bin`,
with a copied `resources` directory beside them. They can also find resources
when CLion runs them from the repository root.

## Adding an assignment

Create a source file in `assignments` with a standard `int main()`, then add one
line to `CMakeLists.txt`:

```cmake
add_cv_program(exercise_3_1 assignments/exercise_3_1.cpp)
```

Reload CMake and the executable will appear in CLion's target dropdown.
