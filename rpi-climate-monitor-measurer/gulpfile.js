var gulp = require('gulp');
var exec = require('child_process').exec;
var buildCommand = "cd src && make && ./performMeasurement";

gulp.task('build',function(){
exec(buildCommand, function (error, stdout, stderr) {
console.log(stdout);
console.log(stderr);
});
});

gulp.task('watch', function() {
gulp.watch('src/*.cpp', ['build']);
gulp.watch('include/*.h', ['build']);
});

gulp.task('default',['build','watch'],function(){});