# Blazar Game Engine
Blazar is a flexable C++ game engine.

## Why a game engine?
This engine is being created for a large scale space game. These types of games generally do not fit well into generic game engines as they try and do *too much*.

Space games are unique due to:
* Large scales
* Large amount of procedural generation
* Minimal authored content
* Large amount of objects.

Additionally, for games played over the network, a large amount of determinism is required. Current game engines make all of this a challenge because they're built for completely authored experiences with no or little procedural generation, and a well known set of objects. 

I've tried for a long time to retrofit a suitable system into the likes of Unity or Unreal, but I either found the system unworkable, unusable, or slow. 

I plan to update approxamately 100,000 entities every tick through 20+ systems.

Additionally, I plan on making games that are 2.5D, though I want the engine to be generic. I need good 2D rendering support with batching and all the goodness that comes with that, but also 3D rendering support. This is why there is a custom renderer instead of reusing a library.

## Goals
* Robust Entity Component System
* ImGUI Based Debugging System
* ImGUI Based Authoring System for Procedural Generation
* Material / Shader system
* Sprite tools, including sprite atlas support.
* Cross Platform (Windows, Mac, Linux)
* Easy asset loading
* Robust VFS supporting different archive formats (including bare files).

## Building
Currently, Blazar only supports Windows. This is because this is at an early stage. Support should be easy to add.

### Requirements:
* Premake
* Conan
* Visual Studio 2019

### Instructions
1. Run setup_project.bat, which will automatically create a build/ directory, install conan libraries, and create a .sln file
2. Open .sln file.
3. Compile. Compile will fail first time. Try again without cleaning.

## Notes
Inspired by The [Hazel](https://github.com/TheCherno/Hazel) Game Engine