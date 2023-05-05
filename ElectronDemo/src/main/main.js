// Modules to control application life and create native browser window
const { app, BrowserWindow } = require('electron')
const { autoUpdater } = require('electron-updater')
const path = require('path')
const url = require('url')
// const { default: installExtension, 
//         REACT_DEVELOPER_TOOLS, 
//         REDUX_DEVTOOLS } = require('electron-devtools-installer')
const logger = require('./log')
let tag = "[main]"

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

// Keep a reference for dev mode
let dev = false

// Broken:
// if (process.defaultApp || /[\\/]electron-prebuilt[\\/]/.test(process.execPath) || /[\\/]electron[\\/]/.test(process.execPath)) {
//   dev = true
// }

if (process.env.NODE_ENV !== undefined && process.env.NODE_ENV === 'development') {
  dev = true
}

function createWindow() {
  // Create the browser window.
  mainWindow = new BrowserWindow({
    width: 1024,
    height: 768,
    show: false,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false
    }
  })

  // and load the index.html of the app.
  let indexPath
  if (dev && process.argv.indexOf('--noDevServer') === -1) {
    indexPath = url.format({
      protocol: 'http:',
      host: 'localhost:8080',
      pathname: 'index.html',
      slashes: true
    })
  } else {
    let dirname = __dirname.replace("/src/main", "")
    let filePath = path.join(dirname, 'dist', 'index.html')
    indexPath = url.format({
      protocol: 'file:',
      pathname: filePath,
      slashes: true
    })
  }

  mainWindow.loadURL(indexPath)
  logger.info(tag, "main.js loaded.", "indexPath:", indexPath)
  native()

  // Don't show until we are ready and loaded
  mainWindow.once('ready-to-show', () => {
    mainWindow.show()

    // Open the DevTools automatically if developing
    if (dev) {
      // [REACT_DEVELOPER_TOOLS, REDUX_DEVTOOLS].forEach(extension => {
      //   installExtension(extension)
      //       .then((name) => logger.debug(tag, `Added Extension: ${name}`))
      //       .catch((err) => logger.error(tag, 'An error occurred: ', err));
      // });
      mainWindow.webContents.openDevTools()
    }
  })

  // Emitted when the window is closed.
  mainWindow.on('closed', function() {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    mainWindow = null
  })
}


const gotTheLock = app.requestSingleInstanceLock()
 
if (!gotTheLock) {
  logger.error(tag, "It was not single instance then app quit.")
  app.quit()
} else {
  // This method will be called when Electron has finished
  // initialization and is ready to create browser windows.
  // Some APIs can only be used after this event occurs.
  app.on('ready', () => {
    createWindow()
  })

  // Quit when all windows are closed.
  app.on('window-all-closed', () => {
    // On macOS it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
      app.quit()
    }
  })

  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (mainWindow === null) {
      createWindow()
    }
  })
}

function native() {
  let native = require('./bindings')("native")
  logger.debug(tag, native.mytest());
  logger.debug(tag, native.youtest('aaa','bbb'));
}
