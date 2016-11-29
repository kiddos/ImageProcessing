var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var start = Math.PI;
var padding = 'same';
var zigzag = [
  [-1, -1], [0, -1], [1, -1], [-1, 0],
  [-1, 1], [0, 1], [1, 1], [1, 0]
];

var kernel = function(x, y, step, w, h, imagePixels) {
  var result = 0;
  var index = y * step + x * 4;
  var val = imagePixels.data[index];

  for (var i = 0 ; i < zigzag.length ; ++i) {
    var newX = x + zigzag[i][0];
    var newY = y + zigzag[i][1];

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
