# Lunaris Bomb Library

This is a bomb library that you can add on your project! It works on both Windows and Linux.

There are some tests to check if it builds correctly and they can be disabled with `BUILD_TESTS OFF`.

## How to add the project to your project

### Using FetchContent

You can create a file like `cmake/installLibrary.cmake` and put in there:

```cmake
include(FetchContent)

FetchContent_Declare(
    lunaris-bomb
    GIT_REPOSITORY https://github.com/LunarisLib/lunaris-bomb.git
    GIT_TAG        (put version here)
)
FetchContent_MakeAvailable(lunaris-bomb)
```

This will allow you to download and link the library like:

```cmake
# ...

include(cmake/installLibrary.cmake) # does the FetchContent

target_link_libraries(YourProjectName PRIVATE
    lunaris::lunaris-bomb
)
```

### Using find_package()

If you get the install version with the lib and headers and want to avoid recompiling the library yourself, you can do

```cmake
# ...

find_package(lunaris-bomb REQUIRED)

target_link_libraries(YourProjectName PRIVATE
    lunaris::lunaris-bomb
)
```

The find_package will try to find the `lunaris-bomb-config.cmake` or similar files that should be available to download in the Release tab.