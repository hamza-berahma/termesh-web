# Termesh

```
████████╗███████╗██████╗ ███╗   ███╗ ███████╗███████╗██╗  ██╗
╚══██╔══╝██╔════╝██╔══██╗████╗ ████║ ██╔════╝██╔════╝██║  ██║
   ██║   █████╗  ██████╔╝██╔████╔██║ █████╗  ███████╗███████║
   ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║ ██╔══╝  ╚════██║██╔══██║
   ██║   ███████╗██║  ██║██║ ╚═╝ ██║ ███████╗███████║██║  ██║
   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝ ╚══════╝╚══════╝╚═╝  ╚═╝
```

ASCII STL renderer that runs in your browser.

[**Live Demo →**](https://termesh.netlify.app)

![Snowflake Demo](snowflake.gif)

## What it does

Converts 3D models (.stl files) into ASCII art. Built with C++ compiled to WebAssembly for performance.

You can load preset models or upload your own files. The renderer outputs 240x80 character displays that automatically resize to fit your screen.

## Tech

- C++17 → WebAssembly (via Emscripten)
- JavaScript ES modules
- Tailwind CSS

The heavy lifting (STL parsing and ASCII conversion) happens in the WASM module. JavaScript handles file I/O and UI.

## Project structure

```
├── index.html
├── models/              # preset .stl files
├── src/
│   ├── index.js         # Emscripten glue code
│   ├── index.wasm       # compiled C++ binary
│   ├── main.js          # app logic
│   ├── tabs.js
│   ├── wasm-module.js
│   └── dom-utils.js
└── styles/
    ├── main.css
    └── production.css
```

## Running locally

You need a web server (CORS requirements):

```bash
python -m http.server
```

Then open `http://localhost:8000`

## Deploy

Push to GitHub, enable Pages in Settings → Pages, deploy from main branch. Done.
