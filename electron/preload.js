
const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('archRender', {
    platform: process.platform,

    importModel: () => ipcRenderer.invoke('dialog:openModel')
});
