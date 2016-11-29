var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var start = Math.PI;
var delta = 8;
var theta = Math.PI * 2 / delta;
var padding = 'same';

var kernel = function(x, y, step, w, h, imagePixels) {
  var result = 0;
  var val = imagePixels.data[y * step + x * 4];
  var meanDiff = 0;
  var newX = 0, newY = 0, nval = 0, index = 0, angle = 0;

  for (var i = 0 ; i < delta ; ++i) {
    angle = i * theta + start;
    newX = range * Math.round(x +
      Math.cos(angle));
    newY = range * Math.round(y +
      Math.sin(angle));

    nval = 0;
    if (padding === 'same') {
      newX = Math.min(Math.max(0, newX), w-1);
      newY = Math.min(Math.max(0, newY), h-1);
      nval = imagePixels.data[newY * step +
        newX * 4];
    } else if (padding == 'zero') {
      if (newX >= 0 && newX < w &&
          newY >= 0 && newY < h) {
        index = newY * step + newX * 4;
        nval = imagePixels.data[index];
      }
    }

    meanDiff += Math.abs(nval - val);
    if (nval >= val) {
      result += (1 << (i * 2));
    }
  }
  meanDiff /= delta;

  for (i = 0 ; i < delta ; ++i) {
    angle = i * theta + start;
    newX = range * Math.round(x +
      Math.cos(angle));
    newY = range * Math.round(y +
      Math.sin(angle));

    nval = 0;
    if (padding === 'same') {
      newX = Math.min(Math.max(0, newX), w-1);
      newY = Math.min(Math.max(0, newY), h-1);
      nval = imagePixels.data[newY * step +
        newX * 4];
    } else if (padding == 'zero') {
      if (newX >= 0 && newX < w &&
          newY >= 0 && newY < h) {
        index = newY * step + newX * 4;
        nval = imagePixels.data[index];
      }
    }

    if (nval >= val + meanDiff) {
      result += (1 << (i * 2 + 1));
    }
  }

  result = result / Math.pow(2, 16) * 255;
  result = Math.min(Math.max(result, 0), 255);
  return result;
};
