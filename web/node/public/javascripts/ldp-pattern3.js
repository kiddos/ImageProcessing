var kernelSize = 3;
var range = parseInt(kernelSize / 2);
var start = Math.PI;
var padding = 'same';
var direction = [
  [-1, -1, -3], [0, -1, -3], [1, -1, 5], [-1, 0, -3],
  [1, 0, 5], [-1, 1, -3], [0, 1, -3], [1, 1, 5]
];

var kernel = function(x, y, step, w, h, imagePixels) {
  var result = 0;
  var index = 0;

  for (var i = 0 ; i < direction.length ; ++i) {
    var newX = x + direction[i][0];
    var newY = y + direction[i][1];

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

    result += direction[i][2] * nval;
  }
  result = Math.min(Math.max(0, result), 255);
  return result;
};
