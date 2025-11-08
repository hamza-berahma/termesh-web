import {
    fileInput,
    modelSelect,
    loadButton,
    displayElement,
    adjustFontSize,
    updateStatus,
    enableControls,
    disableControls,
} from "./dom-utils.js";
import { initializeWasmModule, processSTL } from "./wasm-module.js";

let wasmReady = false;
let hasRendered = false;

async function loadPresetModel() {
    const modelPath = modelSelect.value;
    if (!modelPath) return;
    
    if (hasRendered) {
        sessionStorage.setItem('autoload', modelPath);
        window.location.reload();
        return;
    }
    
    const modelName = modelPath.split("/").pop();
    try {
        updateStatus(`Loading ${modelName}...`, true);
        disableControls();

        displayElement.textContent = "> Fetching model...";

        const response = await fetch(modelPath);
        if (!response.ok) throw new Error(`Failed to load ${modelPath}`);
        const buffer = await response.arrayBuffer();
        await renderData(new Uint8Array(buffer), modelName);
        hasRendered = true;
    } catch (err) {
        updateStatus(`Error: ${err.message}`, false);
        displayElement.textContent = `[ERROR] Could not load ${modelPath}\n${err.message}`;
        enableControls();
    }
}

function loadCustomModel(event) {
    const file = event.target.files[0];
    if (!file) return;
    
    if (hasRendered) {
        window.location.reload();
        return;
    }
    
    const reader = new FileReader();
    reader.onload = async (e) => {
        const buffer = e.target.result;
        await renderData(new Uint8Array(buffer), file.name);
        hasRendered = true;
    };
    reader.onerror = () => {
        updateStatus(`Error reading file: ${reader.error}`, false);
        displayElement.textContent = `[ERROR] Could not read file\n${reader.error}`;
        enableControls();
    };
    updateStatus(`Reading ${file.name}...`, true);
    displayElement.textContent = `> Reading ${file.name}...\n> Please wait...`;
    disableControls();
    reader.readAsArrayBuffer(file);
}

async function renderData(data, modelName) {
    if (!wasmReady) {
        updateStatus(`Waiting for WASM initialization...`, true);
        displayElement.textContent = "> Waiting for WASM...\n> Please wait...";
        
        let attempts = 0;
        while (!wasmReady && attempts < 100) {
            await new Promise(resolve => setTimeout(resolve, 100));
            attempts++;
        }
        
        if (!wasmReady) {
            enableControls();
            throw new Error("WASM initialization timeout");
        }
    }

    updateStatus(`Rendering ${modelName}...`, true);
    displayElement.textContent = "> Processing...\n> Please wait...";
    displayElement.style.fontSize = ""; 

    await new Promise((resolve) => setTimeout(resolve, 10));

    try {
        processSTL(data, modelName);
        updateStatus(`Complete: ${modelName} (reload for new model)`, false);

        await adjustFontSize();
        enableControls();
    } catch (err) {
        updateStatus(`Render Error: ${err.message}`, false);
        displayElement.textContent += `\n[ERROR] ${err.message}`;
        enableControls();
    }
}

const wasmCallbacks = {
    onPrint: (text) => {
        console.log("C++ stdout:", text);
        const currentText = displayElement.textContent;
        if (currentText === "[Awaiting model data...]" || 
            currentText === "[Loading WASM module...]" ||
            currentText.includes("Processing...") || 
            currentText.includes("Waiting for WASM...")) {
            displayElement.textContent = text + "\n";
        } else {
            displayElement.textContent += text + "\n";
        }
    },
    onPrintErr: (text) => {
        console.error("C++ stderr:", text);
        displayElement.textContent += "ERROR: " + text + "\n";
    },
    onRuntimeInitialized: () => {
        console.log("Emscripten runtime is initialized.");
        wasmReady = true;
        enableControls();
        adjustFontSize();
    },
};

loadButton.addEventListener("click", loadPresetModel);
fileInput.addEventListener("change", loadCustomModel);

window.addEventListener("resize", adjustFontSize);

const resolutionMatcher = matchMedia("(resolution)");
resolutionMatcher.addEventListener("change", adjustFontSize);

updateStatus("Loading WASM...", true);
displayElement.textContent = "[Loading WASM module...]";
initializeWasmModule(wasmCallbacks);

window.addEventListener('load', async () => {
    const autoload = sessionStorage.getItem('autoload');
    if (autoload) {
        sessionStorage.removeItem('autoload');
        modelSelect.value = autoload;
        setTimeout(() => loadPresetModel(), 500);
    }
});