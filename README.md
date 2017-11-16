# pGDS

<!-- Designing photonics layouts using the GDSII format often requires large numbers of vertices. pGDS is designed to -->

## Structure and Hierarchy

Todo.

## Usage with MATLAB

* Using `MATLAB`, run `maincompile.m` with `pGDS` as the current folder and `mex/build` on path. This will generate a `.mex(w|maci|a)64` file that acts like a bridge between `MATLAB` and `C++`, along with corresponding wrapper `classdef` `.m` files, one for each `C++` class. Functions that do not belong to a class can be accessed through the static `pGDS` class.
* Any proprietary designs should be suffixed with `devices.(cpp|hpp|m)` or otherwise blacklisted in `.gitignore`.

## Long-Term Plans
 - [x] Integration with a scripting language (`MATLAB`; perhaps `Lua` in the future).
 - [ ] Finish `OpenGL` visualization.
 - [ ] Write a `C++` engine to process updates to `MATLAB` scripts in realtime.
 - [ ] Integration with Atom.
 - [ ] Atomic features (e.g. in `OpenGL`, highlight the device  that is currently selected in `MATLAB` code).
 - [ ] VR Support!

## Optimization Plans
  - [ ] Finish `~O(n)` boolean code.
  - [ ] Improve path thickening algorithm.
  - [ ] Write Bentley–Ottmann-like many-polyline boolean code.
  - [ ] Use references (`&`, `const&`) more to reduce the amount of internal copying between functions.
  - [ ] Multithreading?

<!-- ## Mac/Unix Dependencies -->
<!-- * homebrew `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
* node `brew install node` `npm install --save-dev electron-rebuild`
* glew `brew install glew`
* freetype `brew install freetype` -->
