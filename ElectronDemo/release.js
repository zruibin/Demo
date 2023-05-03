/*
 * release.js
 *
 * Created by Ruibin.Chow on 2023/05/03.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

let fs = require('fs');
let path = require('path')
let fsExtra = require("fs-extra");


let buildPath = "build"
if (!fs.existsSync(buildPath)) {
    fsExtra.ensureDirSync(buildPath)
}

let appPath = path.join(buildPath, "app")
if (fs.existsSync(appPath)) {
    fsExtra.removeSync(appPath)
}
fsExtra.ensureDirSync(appPath)

let copyList = [
    "deps",
    "dist",
    "node_modules",
    "src",
    "package.json"
];

copyList.forEach(value => {
    let dist = path.join(appPath, value)
    console.log(value, "->", dist)
    fsExtra.copySync(value, dist)
});

let srcApp = path.join(appPath, "src", "app")
let modeModulsPath = path.join(appPath, "node_modules")
let removeList = [
    srcApp,
    path.join(modeModulsPath, "electron"),
    path.join(modeModulsPath, "@electron"),
    path.join(modeModulsPath, ".bin"),
    path.join(modeModulsPath, "electron-packager")
];

removeList.forEach(value => {
    console.log("remove: ", value)
    if (fs.existsSync(value)) {
        fsExtra.removeSync(value)
    }
});


// let allModeModulsFile  = [];
// function ThroughDirectory(Directory) {
//     fs.readdirSync(Directory).forEach(File => {
//         const Absolute = path.join(Directory, File);
//         if (fs.statSync(Absolute).isDirectory()) return ThroughDirectory(Absolute);
//         else return allModeModulsFile.push(Absolute);
//     });
// }

// ThroughDirectory(modeModulsPath);
// allModeModulsFile.forEach(value => {
//     value = value.trim()
//     if (value.lastIndexOf("node_gyp_bins/python3") > 0) {
//         fsExtra.removeSync(value)
//     }
// });


