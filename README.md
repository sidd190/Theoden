# Theoden

Theoden is a toy web browser implementation written in C++. It is a learning project aimed at exploring how basic browser components function, including parsing of HTML and CSS and simple rendering of structured content.

## Overview

This project implements a minimal browser engine that can:

* Parse HTML into a DOM model
* Parse simple CSS rules
* Represent document structure and style information
* Render basic content from local HTML/CSS files

The intent is to understand foundational browser internals rather than build a full-featured browser.

## Project Structure

```
Rendering Engine/
├── css.hpp
├── cssParser.cpp
├── cssParser.hpp
├── document.hpp
├── dom/
│   └── dom.hpp
├── example.css
├── example.html
├── htmlParser.cpp
├── htmlParser.hpp
├── main
├── main.cpp
```

### Key Components

* **HTML Parser (`htmlParser.*`)**
  Converts raw HTML text into a structured document object model (DOM).

* **CSS Parser (`cssParser.*`, `css.hpp`)**
  Reads and interprets simple CSS rules, associating them with DOM elements.

* **Document & DOM (`document.hpp`, `dom/`)**
  Defines the DOM data structures used to represent elements, attributes, and text nodes.

* **Rendering / Execution (`main.cpp`)**
  Entry point that loads an example HTML/CSS and processes it through the engine.

* **Examples (`example.html`, `example.css`)**
  Simple demonstration files to exercise the parser and rendering logic.

## Current Progress

* Basic parsing of HTML and CSS is implemented.
* A DOM structure representing parsed content is defined.
* There is minimal rendering logic connected to the parsed output.
* The example files showcase how the engine processes markup and style.

## Limitations

* No JavaScript support.
* Limited CSS feature coverage.
* Rendering logic is basic and not layout-complete.
* Does not fetch content over network; only local files are processed.

## Build & Run

To compile and run the code:

```bash
# compile
g++ -std=c++17 Rendering\ Engine/*.cpp -o theoden

# run
./theoden
```

Replace paths/flags as appropriate for your build environment.

## Goals & Future Work

Possible future directions include:

* Expanding CSS selector and property support
* Implementing basic layout algorithms (block layout, inline flow)
* Rendering formatted output beyond structural parsing
* Adding tests for parser and DOM components
