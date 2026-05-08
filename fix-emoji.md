# Fix Emoji Display on Windows CMake Project Terminal

Here's the code to fix emoji display issues on Windows:

## 1. In CMakeLists.txt (after `set(CMAKE_CXX_STANDARD 17)`)

```cmake
# Enable UTF-8 encoding for MSVC compiler
if(MSVC)
    add_compile_options(/utf-8)
endif()
```

## 2. At the top of main.cpp (with other includes)

```cpp
#include <windows.h>
```

## 3. In main() function (first lines of the function)

```cpp
int main()
{
    // Enable UTF-8 support on Windows console
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    // ... rest of your code
}
```

## What These Do

- **Compiler flag**: Tells MSVC compiler to read source files as UTF-8
- **SetConsoleCP(65001)**: Configures Windows console input to accept UTF-8
- **SetConsoleOutputCP(65001)**: Configures Windows console output to display UTF-8 characters (emojis)

These three changes together enable proper emoji display in your terminal output.
