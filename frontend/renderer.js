
const importButton = document.getElementById('importModel');
const sceneObjects = document.getElementById('sceneObjects');
const consoleOutput = document.getElementById('consoleOutput');
const viewport = document.getElementById('viewport');

let importedModels = [];

function updateViewport() {
    const bounds = viewport.getBoundingClientRect();
    window.archRender.setViewport({
        x: bounds.left,
        y: bounds.top,
        width: bounds.width,
        height: bounds.height
    });
}

new ResizeObserver(updateViewport).observe(viewport);
window.addEventListener('resize', updateViewport);

function logMessage(message) {
    const log = document.createElement('p');
    log.textContent = `> ${message}`;
    consoleOutput.appendChild(log);
}

importButton.addEventListener('click', async () => {
    try {
        const modelPath = await window.archRender.importModel();

        if (!modelPath) {
            logMessage('Model import cancelled.');
            return;
        }

        const modelName = modelPath.split('/').pop();

        importedModels.push({
            name: modelName,
            path: modelPath
        });

        // Remove empty message
        sceneObjects.querySelector('.empty-message')?.remove();

        // Add model to scene hierarchy
        const object = document.createElement('div');
        object.textContent = `📦 ${modelName}`;
        object.classList.add('scene-object');

        sceneObjects.appendChild(object);

        logMessage(`Selected model: ${modelName}`);

    } catch (error) {
        logMessage(`Import error: ${error.message}`);
        console.error(error);
    }
});
