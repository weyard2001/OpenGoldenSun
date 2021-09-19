# Open Golden Sun Project
This project aims to recreate parts of Golden Sun and potentially spawn new life into the series again after its last release over a decade ago. The engine and game are made primarily using C/C++. A quick demo of the current progress is available at https://www.youtube.com/watch?v=4As5fNnf6LM

## Libraries and Tools Used
* **SDL2** for input handling, outputting audio, and rendering graphics. 
* **tmxlite** to parse data from .tmx map files created in Tiled Map Editor.
* **glm** as a math library.
* **imgui** for user interfaces and debug information. 
* **Tiled Map Editor** to create maps.
* **Atrius' Golden Sun Editor** to extract sprites and other game data from Golden Sun.

## Requirements
If you would like to run the project yourself, you will need the following libraries on your machine.
* SDL2, SDL2_image, SDL2_mixer, SDL2_ttf

## Credits and Acknowledgements
Golden Sun and all images and sounds in this project belong to Nintendo and Camelot. This project does not own any rights to the Golden Sun franchise nor the assets used to create the official games.

Many of the technical details unpacked from the original game were compiled by the community over at http://goldensunhacking.net/. If you're a fan of the Golden Sun series as well, please check them out. They're still working on fanbased Golden Sun content to this day.

This is my first attempt at working on a project of this size (as well as it being my first attempt at creating a game in C++ and implementing a design pattern known as Entity Component System). I had followed some of the engine structure from https://github.com/fallahn/xygine (also the author who made tmxlite, the tmx parser used in this project) in order to implement parts of the engine in this project.