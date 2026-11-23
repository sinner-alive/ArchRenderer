
const importButton = document.getElementById('importModel');
const consoleOutput = document.getElementById('consoleOutput');
const viewportEl = document.querySelector("#viewport") ;

let importedModels = [];

function logMessage(message) {
    const log = document.createElement('p');
    log.textContent = `> ${message}`;
    consoleOutput.appendChild(log);
}

// this fucntion import the current model path
importButton.addEventListener('click', async () => {
    try {
        const modelPath = await window.archRender.importModel();

        if (!modelPath) {
            logMessage('Model import cancelled.');
            return;
        }

        const modelName = modelPath.split(/[\\/]/).pop() || modelPath;

        importedModels.push({
            name: modelName,
            path: modelPath
        });

        // print model name 
        const nameEl = document.querySelector('.model-name');
        if (nameEl) nameEl.textContent = modelName;
        logMessage(`Selected model: ${modelName}`);

    } catch (error) {
        logMessage(`Import error: ${error.message}`);
        console.error(error);
    }
});
