var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var start = Math.PI;
var alpha = 3;
var padding = 'same';
var zigzag = [
  [-1, -1], [0, -1], [1, -1], [-1, 0],
  [-1, 1], [0, 1], [1, 1], [1, 0]
];

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
  var newX = 0, newY = 0, nval = 0, index = 0;
  var newX2 = 0, newY2 = 0, nval2 = 0, index2 = 0;
  var values = [];

  for (var i = 0 ; i < zigzag.length ; ++i) {
    newX = x + zigzag[i][0];
    newY = y + zigzag[i][1];

    var thisValue = getNeighbor(newX, newY, step, w, h, imagePixels);
    var value = 0;

    for (var j = 0 ; j < zigzag.length ; ++j) {
      newX2 = newX + zigzag[j][0];
      newY2 = newY + zigzag[j][1];

      var neighborValue = getNeighbor(newX2, newY2, step, w, h, imagePixels);
      if (neighborValue >= thisValue) {
        value += (1 << i);
      }
    }
    values.push(value);
  }

  var result = 0;
  for (i = 0 ; i < zigzag.length ; ++i) {
    var i0 = i;
    var i1 = (i + alpha) % zigzag.length;
    if (values[i0] >= values[i1]) {
      result += (1 << i);
    }
  }
  return result;
};
