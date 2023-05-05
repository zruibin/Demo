/*
 * log.js
 *
 * Created by Ruibin.Chow on 2023/05/05.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

var logger = console;
var isElectron = false;
if (navigator.userAgent.toLowerCase().indexOf(' electron/') > -1){
  isElectron = true;
  logger = require('electron-log');
  logger.transports.file.level = 'debug'
  logger.transports.file.maxSize = 1002430 // 最大不超过10M
  logger.transports.file.format = '[{y}-{m}-{d} {h}:{i}:{s}.{ms}]{scope}{text}' // 设置文件内容格式
  let date = new Date()
  date = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate()
  logger.transports.file.fileName = date + '.log' // 创建文件名格式为 '时间.log' (2023-02-01.log)
} 

module.exports = {
  info() {
    logger.info("[I]", ...arguments);
  },                   
  warn() {
    logger.warn("[W]", ...arguments);
  },
  error() {
    logger.error("[E]", ...arguments);
  },
  debug() {
    if (isElectron) {
      logger.debug("[D]", ...arguments);
      console.log("[D]", ...arguments);
    } else {
      logger.log("[D]", ...arguments);
    }
  },
  verbose() {
    logger.verbose("[V]", ...arguments);
  },
  silly() {
    logger.silly("[S]", ...arguments);
  }
};
