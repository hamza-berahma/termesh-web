export function initializeWasmModule(o) {
    window.Module = {
        preRun: [() => console.log("Module preRun: Setting up filesystem.")],
        postRun: [],
        print: o.onPrint || ((text) => console.log(text)),
        printErr: o.onPrintErr || ((text) => console.error(text)),
        onRuntimeInitialized: o.onRuntimeInitialized || (() => console.log("Runtime initialized.")),
        noInitialRun: true,
    };
    
    const script = document.createElement("script");
    script.src = "src/index.js";
    script.onerror = () => {
        o.onPrintErr("Failed to load WASM script (index.js).");
    };
    document.body.appendChild(script);
}

export function processSTL(data, filename) {
    if (!window.Module || !window.Module.FS) {
        console.error("Error: WASM Module or Filesystem not ready.");
        throw new Error("WASM Module not ready.");
    }

    try {
        Module.FS.writeFile("/model.stl", data);
        Module.callMain();
    } catch (err) {
        console.error(`Error processing STL in WASM: ${err}`);
        throw new Error(`WASM execution failed: ${err.message}`);
    }
}