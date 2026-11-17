
import { app, BrowserWindow, ipcMain, dialog } from 'electron';
import path from 'node:path';
import { spawn } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

let engineProcess = null;

function startEngine() {
    const enginePath = path.join(
        __dirname,
        '../engine/build/ArchRender'
    );

    engineProcess = spawn(enginePath, [], {
        cwd: path.join(__dirname, '..')
    });

    engineProcess.stdout.on('data', (data) => {
        console.log(`[Engine]: ${data}`);
    });

    engineProcess.stderr.on('data', (data) => {
        console.error(`[Engine Error]: ${data}`);
    });

    engineProcess.on('error', (error) => {
        console.error('Failed to launch engine:', error);
    });

    engineProcess.on('close', (code) => {
        console.log(`Engine exited with code ${code}`);
        engineProcess = null;
    });
}

ipcMain.handle('dialog:openModel', async () => {
    const result = await dialog.showOpenDialog({
        title: 'Import 3D Model',
        properties: ['openFile'],
        filters: [
            {
                name: '3D Models',
                extensions: ['obj', 'fbx', 'gltf', 'glb', 'dae', 'stl']
            }
        ]
    });

    if (result.canceled || result.filePaths.length === 0) {
        return null;
    }

    return result.filePaths[0];
});



function createWindow() {
    const mainWindow = new BrowserWindow({
        width: 1600,
        height: 1000,
        minWidth: 1100,
        minHeight: 700,
        title: 'ArchRender',
        backgroundColor: '#17191f',

        webPreferences: {
            preload: path.join(__dirname, 'preload.js'),
            contextIsolation: true,
            nodeIntegration: false
        }
    });

    mainWindow.loadFile(
        path.join(__dirname, '../frontend/index.html')
    );

    mainWindow.maximize();
}

app.whenReady().then(() => {
    createWindow();
    startEngine();

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow();
        }
    });
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }

});

app.on('before-quit', () => {
    if (engineProcess) {
        engineProcess.kill();
    }
});
