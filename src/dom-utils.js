export const fileInput = document.getElementById("fileInput");
export const modelSelect = document.getElementById("modelSelect");
export const loadButton = document.getElementById("loadButton");
export const displayElement = document.getElementById("display"); // This is a <pre>

const statusElement = document.getElementById("status");

export function updateStatus(message, isLoading = false) {
    if (!statusElement) return;

    statusElement.textContent = message;
    if (isLoading) {
        statusElement.classList.add("cursor-blink");
    } else {
        statusElement.classList.remove("cursor-blink");
    }
}

export function enableControls() {
    updateStatus("Ready. Select a model or upload an STL file.", false);
    if (fileInput) fileInput.disabled = false;
    if (modelSelect) modelSelect.disabled = false;
    if (loadButton) loadButton.disabled = false;
}

export async function adjustFontSize() {
    await new Promise((resolve) => requestAnimationFrame(resolve));

    const container = displayElement.parentElement;
    if (!container || !displayElement) return;

    const containerWidth = container.clientWidth;
    const containerHeight = container.clientHeight;

    if (containerWidth === 0 || containerHeight === 0) {
        return;
    }

    const CONTENT_CHAR_WIDTH = 240;
    const CONTENT_CHAR_HEIGHT = 80;

    const { fontAspectRatio } = await getFontMetrics();

    const fontSizeBasedOnWidth = containerWidth / (CONTENT_CHAR_WIDTH * fontAspectRatio);

    const fontSizeBasedOnHeight = containerHeight / CONTENT_CHAR_HEIGHT;

    const newFontSize = Math.floor(Math.min(fontSizeBasedOnWidth, fontSizeBasedOnHeight));

    if (newFontSize > 0) {
        displayElement.style.fontSize = `${newFontSize}px`;
    }
}

let fontMetricsCache = null;
async function getFontMetrics() {
    if (fontMetricsCache) {
        return fontMetricsCache;
    }

    try {
        await document.fonts.load("100px 'IBM Plex Mono'");
    } catch (err) {
        console.warn("Font loading timed out, using fallback metrics.", err);
    }

    const tempSpan = document.createElement("span");
    tempSpan.innerHTML = "0";
    tempSpan.style.fontFamily = "'IBM Plex Mono', monospace";
    tempSpan.style.fontSize = "100px";
    tempSpan.style.lineHeight = "1";
    tempSpan.style.position = "absolute";
    tempSpan.style.visibility = "hidden";
    tempSpan.style.left = "-9999px";
    document.body.appendChild(tempSpan);

    const charPixelWidth = tempSpan.clientWidth;
    const charPixelHeight = tempSpan.clientHeight;

    document.body.removeChild(tempSpan);

    fontMetricsCache = {
        charPixelWidth: charPixelWidth,
        charPixelHeight: charPixelHeight,
        fontAspectRatio: charPixelWidth / charPixelHeight,
    };

    return fontMetricsCache;
}
