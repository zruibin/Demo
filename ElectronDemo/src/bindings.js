/*
 * bindings.js
 *
 * Created by Ruibin.Chow on 2023/05/03.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

function bindings(name) {
  // Argument surgery
  if (!name) {
    name = "native";
  }


  return  require('bindings')({ bindings: name, try: [['module_root', 'deps', 'bindings']] })
}

module.exports = exports = bindings;