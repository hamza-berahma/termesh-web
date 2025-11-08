export function initializeWasmModule(o) {
    window.Module = {
        preRun: [() => console.log("Module preRun: Setting up filesystem.")],
        postRun: [],
        print: o.onPrint || ((o) => console.log(o)),
        printErr: o.onPrintErr || ((o) => console.error(o)),
        onRuntimeInitialized: o.onRuntimeInitialized || (() => console.log("Runtime initialized.")),
        noInitialRun: !0,
    };
    const e = document.createElement("script");
    (e.src = "src/index.js"),
        (e.onerror = () => {
            o.onPrintErr("Failed to load WASM script (index.js).");
        }),
        document.body.appendChild(e);
}
export function processSTL(o, e) {
    if (!window.Module || !window.Module.FS)
        throw (
            (console.error("Error: WASM Module or Filesystem not ready."),
            new Error("WASM Module not ready."))
        );
    try {
        Module.FS.writeFile("/model.stl", o), Module.callMain();
    } catch (o) {
        throw (
            (console.error(`Error processing STL in WASM: ${o}`),
            new Error(`WASM execution failed: ${o.message}`))
        );
    }
}
