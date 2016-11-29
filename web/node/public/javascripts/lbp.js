var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var start = Math.PI;
var delta = 8;
var theta = Math.PI * 2 / delta;
var padding = 'same';

var kernel = function(x, y, step, w, h, imagePixels) {
  var result = 0;
  var index = y * step + x * 4;
  var val = imagePixels.data[index];
  for (var i = 0 ; i < delta ; ++i) {
    var angle = i * theta + start;
    var newX = range * Math.round(x +
      Math.cos(angle));
    var newY = range * Math.round(y +
      Math.sin(angle));

    var nval = 0;
    if (padding === 'same') {
      newX = Math.min(Math.max(0, newX), w-1);
      newY = Math.min(Math.max(0, newY), h-1);
      index = newY * step + newX * 4;
      nval = imagePixels.data[index];
    } else if (padding == 'zero') {
      if (newX >= 0 && newX < w &&
          newY >= 0 && newY < h) {
        index = newY * step + newX * 4;
        nval = imagePixels.data[index];
      }
    }

    if (nval >= val) {
      result += (1 << i);
    }
  }
  return result;
};
