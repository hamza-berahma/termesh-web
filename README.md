# Termesh - ASCII STL Renderer (Web)

```
████████╗███████╗██████╗ ███╗   ███╗ ███████╗███████╗██╗  ██╗
╚══██╔══╝██╔════╝██╔══██╗████╗ ████║ ██╔════╝██╔════╝██║  ██║
   ██║   █████╗  ██████╔╝██╔████╔██║ █████╗  ███████╗███████║
   ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║ ██╔══╝  ╚════██║██╔══██║
   ██║   ███████╗██║  ██║██║ ╚═╝ ██║ ███████╗███████║██║  ██║
   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝ ╚══════╝╚══════╝╚═╝  ╚═╝
```

<p align="center">
<i>"Rendering your 3D world, one character at a time."</i>
</p>

<p align="center">
<a href="https://termesh.netlify.app">🌐 Live Demo</a>
</p>

<p align="center">
<img alt="C++" src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus"/>
<img alt="WASM" src="https://img.shields.io/badge/WebAssembly-654FF0?style=for-the-badge&logo=webassembly"/>
<img alt="JavaScript" src="https://img.shields.io/badge/JavaScript-ES_Modules-F7DF1E?style=for-the-badge&logo=javascript"/>
<img alt="Tailwind" src="https://img.shields.io/badge/Tailwind_CSS-06B6D4?style=for-the-badge&logo=tailwindcss"/>
</p>

## 🎨 Demo

Here's a render of the snowflake.stl model, captured as a GIF:

![Snowflake Demo](snowflake.gif)

## 📖 Overview

Termesh is a web-based application that renders 3D models from `.stl` files into ASCII art. It uses a C++17 core logic compiled to WebAssembly (WASM) to perform the computationally intensive conversion, allowing it to run entirely within the browser.

**🚀 Try it live at [termesh.netlify.app](https://termesh.netlify.app)**

The user can either select from a list of preset models or upload their own `.stl` file. The resulting ASCII render is displayed in a responsive, terminal-themed interface that adjusts the font size to fit the art to the user's screen.

## ✨ Features

- **WASM-Powered**: Leverages a C++ backend compiled to WebAssembly for high-performance STL parsing and ASCII generation
- **Preset Models**: Includes a selection of 14 preset `.stl` models for immediate rendering
- **Custom Upload**: Allows users to upload and render their own `.stl` files
- **Terminal Aesthetic**: A retro, terminal-inspired UI built with Tailwind CSS
- **Responsive ASCII Display**: The output automatically adjusts its font size to ensure the entire render (240x80 characters) is visible within the output panel, regardless of screen size
- **Tabbed Interface**: Cleanly separates the "Preset Model" and "Custom Upload" workflows

## 🛠️ Technology Stack

- **Frontend**: HTML5, CSS3, JavaScript (ES Modules)
- **Styling**: Tailwind CSS (production.css) & Custom CSS (main.css)
- **Backend**: C++17 compiled to WebAssembly using Emscripten

### Core Logic

- JavaScript handles file loading (fetch/FileReader), DOM manipulation, and UI event handling
- The WASM module handles file processing, 3D-to-ASCII conversion logic, and outputs the result via stdout

## 🔧 How It Works

<details>
<summary><strong>Click to expand</strong></summary>

1. **WASM Initialization**: `main.js` calls `initializeWasmModule` (from `wasm-module.js`) which loads the Emscripten-generated `index.js` glue code. This, in turn, loads the `index.wasm` binary and sets up the virtual Module object.

2. **Callback Setup**: `main.js` provides callbacks to the WASM module, specifically `onPrint`. This callback will fire every time the C++ code calls `printf` or `std::cout`.

3. **Model Loading**:
   - **Preset**: The user selects a model and clicks "LOAD". `main.js` fetches the `.stl` file from the `/models` directory as an ArrayBuffer.
   - **Custom**: The user selects a local `.stl` file. `main.js` uses a FileReader to read the file as an ArrayBuffer.

4. **Processing**: The ArrayBuffer (as a Uint8Array) is passed to `processSTL` (in `wasm-module.js`).

5. **WASM Filesystem**: `processSTL` writes the model data to the virtual filesystem inside the WASM module (e.g., as `/model.stl`).

6. **WASM Execution**: `processSTL` then calls `Module.callMain()`, triggering the C++ program's main function.

7. **Render**: The C++ backend parses the STL file from its virtual filesystem, performs the 3D-to-ASCII conversion, and prints the result, line by line, to stdout.

8. **Display**: Each print call triggers the `onPrint` callback in `main.js`, which appends the line of ASCII text to the `<pre id="display">` element, rendering the final image.

9. **Fit to Screen**: After rendering, `adjustFontSize` (from `dom-utils.js`) calculates and applies the optimal font size to make the 240x80 character output fit perfectly in its container.

</details>

## 📁 Project Structure

<details>
<summary><strong>Click to expand</strong></summary>

```
.
├── index.html         # The main HTML file, container for the app
├── snowflake.gif      # Demo GIF for the README
├── models             # Directory for preset .stl files
│   ├── arch-btw.stl
│   ├── basebat.stl
│   ├── coin.stl
│   ├── cube.stl
│   ├── cylinder.stl
│   ├── donut.stl
│   ├── ellipsoid.stl
│   ├── polyhedron.stl
│   ├── pyramid.stl
│   ├── snowflake.stl
│   ├── sphere.stl
│   ├── star.stl
│   └── termesh.stl
├── src                # JavaScript source files
│   ├── dom-utils.js     # DOM element selectors & UI helper functions
│   ├── index.js         # Emscripten-generated WASM "glue" code
│   ├── index.wasm       # The compiled C++ WebAssembly binary
│   ├── main.js          # Main application logic
│   ├── tabs.js          # Simple logic for the tabbed interface
│   └── wasm-module.js   # Helper module to initialize and interact with WASM
└── styles             # CSS stylesheets
    ├── main.css         # Custom styles (font, terminal borders, animations)
    └── production.css   # Generated Tailwind CSS file
```

</details>

## 🚀 Running & Deployment

### Running Locally

Because this project loads WebAssembly modules and uses `fetch()` to get preset models, it must be run from a local web server due to browser security policies (CORS). You cannot just open the `index.html` file from your filesystem.

1. Clone the repository
2. Start a local server in the root directory. A simple way is:

```bash
# For Python 3
python -m http.server
```

Or, if you have Node.js and `serve` (`npm install -g serve`):

```bash
serve .
```

3. Open your browser and navigate to the address provided (e.g., `http://localhost:8000`)

### Quick Deploy to GitHub Pages

This is a static site, so deploying is free and easy with GitHub Pages.

1. Push all your code to the `main` (or `master`) branch of your GitHub repository
2. Go to your repo's **Settings > Pages**
3. Under **Build and deployment**, select **Deploy from a branch**
4. Set the **Branch** to `main` and the folder to `/` (root)
5. Click **Save**. Your site will be live in a minute or two!


---

<p align="center">Made with ❤️ and ASCII art</p>