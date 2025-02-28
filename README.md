# ShakEngine

Simple and easy to use 2D game engine developed by Matteo Cavagnino.

## Getting Started

There are two way of getting started,

1. Just make a game using Cmake FetchContent and your config
```
include(FetchContent)
FetchContent_Declare(
  ShakEngine
  GIT_REPOSITORY https://github.com/Teo7297/ShakEngine.git
  GIT_TAG master
  )
FetchContent_MakeAvailable(ShakEngine)
```

this way (also submodules work) you can setup your own configuration.

2. Automatically generate a new vscode project from the engine repo(recommended)

Simply clone this repository:

`git clone --depth=1 https://github.com/Teo7297/ShakEngine.git`

and run the batch script:

`scripts/GenerateNewProject.bat`

This will generate a new project already setted up and ready to build.