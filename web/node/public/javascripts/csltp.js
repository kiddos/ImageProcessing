var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var start = Math.PI;
var delta = 4;
var theta = Math.PI * 2 / delta;
var padding = 'same';
var threshold = 10;

var kernel = function(x, y, step, w, h, imagePixels) {
  var result = 0;
  var index = y * step + x * 4;
  var val = imagePixels.data[index];
  for (var i = 0 ; i < delta ; ++i) {
    var angle = i * theta + start;
    var newX1 = range * Math.round(x +
      Math.cos(angle));
    var newY1 = range * Math.round(y +
      Math.sin(angle));

    angle = i * theta + start + Math.PI;
    var newX2 = range * Math.round(x +
      Math.cos(angle));
    var newY2 = range * Math.round(y +
      Math.sin(angle));

    var nval1 = 0;
    if (padding === 'same') {
      newX1 = Math.min(Math.max(0, newX1), w-1);
      newY1 = Math.min(Math.max(0, newY1), h-1);
      index = newY1 * step + newX1 * 4;
      nval1 = imagePixels.data[index];

      newX2 = Math.min(Math.max(0, newX2), w-1);
      newY2 = Math.min(Math.max(0, newY2), h-1);
      index = newY2 * step + newX2 * 4;
      nval2 = imagePixels.data[index];
    } else if (padding == 'zero') {
      if (newX >= 0 && newX < w &&
          newY >= 0 && newY < h) {
        index = newY1 * step + newX1 * 4;
        nval1 = imagePixels.data[index];

        index = newY2 * step + newX2 * 4;
        nval2 = imagePixels.data[index];
      }
    }

    if (nval1 >= nval2 + threshold) {
      result += 2 * Math.pow(3, i);
    } else if (nval1 >= nval2 - threshold) {
      result += Math.pow(3, i);
    }
  }

  result = result / 120 * 255;
  result = Math.min(Math.max(result, 0), 255);
  return result;
};
