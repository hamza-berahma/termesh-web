import {
    fileInput,
    modelSelect,
    loadButton,
    displayElement,
    adjustFontSize,
    updateStatus,
    enableControls,
} from "./dom-utils.js";
import { initializeWasmModule, processSTL } from "./wasm-module.js";

async function loadPresetModel() {
    const modelPath = modelSelect.value;
    if (!modelPath) return;
    const modelName = modelPath.split("/").pop();
    try {
        updateStatus(`Loading ${modelName}...`, true);

        displayElement.textContent = "> Fetching model...";

        const response = await fetch(modelPath);
        if (!response.ok) throw new Error(`Failed to load ${modelPath}`);
        const buffer = await response.arrayBuffer();
        await renderData(new Uint8Array(buffer), modelName);
    } catch (err) {
        updateStatus(`Error: ${err.message}`, false);
        displayElement.textContent = `[ERROR] Could not load ${modelPath}\n${err.message}`;
    }
}

function loadCustomModel(event) {
    const file = event.target.files[0];
    if (!file) return;
    const reader = new FileReader();
    reader.onload = async (e) => {
        const buffer = e.target.result;
        await renderData(new Uint8Array(buffer), file.name);
    };
    reader.onerror = () => {
        updateStatus(`Error reading file: ${reader.error}`, false);
        displayElement.textContent = `[ERROR] Could not read file\n${reader.error}`;
    };
    updateStatus(`Reading ${file.name}...`, true);
    displayElement.textContent = `> Reading ${file.name}...\n> Please wait...`;
    reader.readAsArrayBuffer(file);
}

async function renderData(data, modelName) {
    updateStatus(`Rendering ${modelName}...`, true);
    displayElement.textContent = "> Processing...\n> Please wait...";
    displayElement.style.fontSize = ""; // Reset font size

    await new Promise((resolve) => setTimeout(resolve, 10));

    try {
        processSTL(data, modelName);
        updateStatus(`Complete: ${modelName}`, false);

        await adjustFontSize();
    } catch (err) {
        updateStatus(`Render Error: ${err.message}`, false);
        displayElement.textContent += `\n[ERROR] ${err.message}`;
    }
}

const wasmCallbacks = {
    onPrint: (text) => {
        console.log("C++ stdout:", text);
        const currentText = displayElement.textContent;
        if (currentText === "[Awaiting model data...]" || currentText.includes("Processing...")) {
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
        enableControls();

        adjustFontSize();
    },
};

loadButton.addEventListener("click", loadPresetModel);
fileInput.addEventListener("change", loadCustomModel);

window.addEventListener("resize", adjustFontSize);

const resolutionMatcher = matchMedia("(resolution)");
resolutionMatcher.addEventListener("change", adjustFontSize);

initializeWasmModule(wasmCallbacks);
