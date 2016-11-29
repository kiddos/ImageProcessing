var kernel = function(x, y, w, h, imagePixels) {
  var step = w * 4;
  var r = imagePixels.data[y * step + x * 4];
  var g = imagePixels.data[y * step + x * 4 + 1];
  var b = imagePixels.data[y * step + x * 4 + 2];

  var V = Math.max(Math.max(r, g), b);
  var minval = Math.min(Math.min(r, g), b);
  var S = parseInt((v - minval) / v);

  var H = 0;
  if (v == r) {
    H = 60 * (g - b) / (v - minval);
  } else if (v == g) {
    H = 120 + 60 * (b - r) / (v - minval);
  } else {
    H = 240 + 60 * (r - g) / (v - minval);
  }
  H = parseInt(Math.min(Math.max(h, 0), 255));
  return [H, S, V];
};
