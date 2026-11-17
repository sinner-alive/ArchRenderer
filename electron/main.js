
import { app, BrowserWindow, ipcMain, dialog } from 'electron';
import { existsSync } from 'node:fs';
import path from 'node:path';
import { spawn, spawnSync } from 'node:child_process';
import { createRequire } from 'node:module';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const projectRoot = path.join(__dirname, '..');

let engineProcess = null;
let nativeRenderer = null;
let renderTimer = null;
const require = createRequire(import.meta.url);

function buildNativeRenderer(addonPath) {
    if (existsSync(addonPath)) {
        return;
    }

    const buildDirectory = path.join(projectRoot, 'engine/build');
    const configure = spawnSync('cmake', ['-S', path.join(projectRoot, 'engine'), '-B', buildDirectory], {
        cwd: projectRoot,
        stdio: 'inherit'
    });
    if (configure.error) {
        throw configure.error;
    }
    if (configure.status !== 0) {
        throw new Error(`CMake configuration failed with exit code ${configure.status}.`);
    }

    const build = spawnSync(
        'cmake',
        ['--build', buildDirectory, '--target', 'ArchRenderNative', '--parallel', '4'],
        { cwd: projectRoot, stdio: 'inherit' }
    );
    if (build.error) {
        throw build.error;
    }
    if (build.status !== 0) {
        throw new Error(`Native renderer build failed with exit code ${build.status}.`);
    }
}

function stopEngine() {
    if (renderTimer) {
        clearInterval(renderTimer);
        renderTimer = null;
    }
    if (nativeRenderer) {
        nativeRenderer.stop();
        nativeRenderer = null;
    }
    if (engineProcess) {
        engineProcess.kill();
        engineProcess = null;
    }
}

function startEngine(mainWindow) {
    if (process.platform === 'darwin') {
        const addonPath = path.join(__dirname, '../engine/build/ArchRenderNative.node');
        buildNativeRenderer(addonPath);
        nativeRenderer = require(addonPath);
        nativeRenderer.start(mainWindow.getNativeWindowHandle(), projectRoot);
        renderTimer = setInterval(() => {
            if (nativeRenderer.shouldClose()) {
                clearInterval(renderTimer);
                renderTimer = null;
                nativeRenderer.stop();
                nativeRenderer = null;
                return;
            }
            nativeRenderer.render();
        }, 16);
        return;
    }

    const enginePath = path.join(
        __dirname,
        '../engine/build/ArchRender'
    );

    engineProcess = spawn(enginePath, [], {
        cwd: projectRoot
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

ipcMain.on('viewport:set', (_event, bounds) => {
    if (!nativeRenderer ||
        !bounds ||
        ![bounds.x, bounds.y, bounds.width, bounds.height].every(Number.isFinite) ||
        bounds.width <= 0 ||
        bounds.height <= 0) {
        return;
    }

    nativeRenderer.setViewport(
        Math.round(bounds.x),
        Math.round(bounds.y),
        Math.round(bounds.width),
        Math.round(bounds.height)
    );
});

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
    mainWindow.on('closed', () => {
        if (process.platform === 'darwin') {
            stopEngine();
        }
    });
    return mainWindow;
}

app.whenReady().then(() => {
    startEngine(createWindow());

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            startEngine(createWindow());
        }
    });
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }

});

app.on('before-quit', () => {
    stopEngine();
});
