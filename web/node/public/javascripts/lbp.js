var kernelSize = 3;
var start = Math.PI;
var delta = 8;
var theta = Math.PI * 2 / delta;
var padding = 'same';

var kernel = function(x, y, step, w, h,
                      imagePixels) {
  var result = 0;
  var val =
      imagePixels.data[y * step + x * 4];
  for (var i = 0 ; i < delta ; ++i) {
    var newX = Math.round(x +
      Math.cos(i * theta));
    var newY = Math.round(y +
      Math.sin(i * theta));

    var nval = 0;
    if (padding === 'same') {
      newX = Math.min(Math.max(0, newX), w-1);
      newY = Math.min(Math.max(0, newY), h-1);
      nval = imagePixels.data[newY * step +
        newX * 4];
    } else if (padding == 'zero') {
      if (newX >= 0 && newX < w &&
          newY >= 0 && newY < h) {
        nval = imagePixels.data[newY * step +
          newX * 4];
      }
    }

    if (nval >= val) {
      result += (1 << i);
    }
  }
  return result;
};
