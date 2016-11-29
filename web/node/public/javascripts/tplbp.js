var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var alpha = 3;
var start = Math.PI;
var delta = 8;
var theta = Math.PI * 2 / delta;
var padding = 'same';

var getNeighbor = function(x, y, step, w, h, imagePixels) {
  var value = 0;
  var index = 0;
  if (padding === 'same') {
    x = Math.min(Math.max(0, x), w-1);
    y = Math.min(Math.max(0, y), h-1);
    index = y * step + x * 4;
    value = imagePixels.data[index];
  } else if (padding == 'zero') {
    if (newX >= 0 && newX < w &&
        newY >= 0 && newY < h) {
      index = newY * step + newX * 4;
      value = imagePixels.data[index];
    }
  }
  return value;
};

var kernel = function(x, y, step, w, h, imagePixels) {
  var val = imagePixels.data[y * step + x * 4];
  var newX = 0, newY = 0, nval = 0, index = 0;
  var newX2 = 0, newY2 = 0, nval2 = 0, index2 = 0;
  var values = [];

  for (var i = 0 ; i < delta ; ++i) {
    var angle = i * theta + start;
    newX = range * Math.round(x +
      Math.cos(angle));
    newY = range * Math.round(y +
      Math.sin(angle));

    var thisValue = getNeighbor(newX, newY, step, w, h, imagePixels);
    var value = 0;

    for (var j = 0 ; j < delta ; ++j) {
      var angle2 = j * theta + start;
      newX2 = range * Math.round(newX +
        Math.cos(angle2));
      newY2 = range * Math.round(newY +
        Math.sin(angle2));

      var neighborValue = getNeighbor(newX2, newY2, step, w, h, imagePixels);
      if (neighborValue >= thisValue) {
        value += (1 << i);
      }
    }
    values.push(value);
  }

  var result = 0;
  for (i = 0 ; i < delta ; ++i) {
    var i0 = i;
    var i1 = (i + alpha) % delta;
    if (values[i0] >= values[i1]) {
      result += (1 << i);
    }
  }

  return result;
};
