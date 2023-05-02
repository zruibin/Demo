const gulp = require('gulp')
const watch = require('gulp-watch')
const electron = require('electron-connect').server.create()

gulp.task('watch:electron', function () {
  electron.start()
  watch(['./*.js'], electron.restart)
  watch(['./*.{html,js,css}'], electron.reload)
})