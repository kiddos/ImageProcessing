var kernel = function(x, y, w, h, imagePixels) {
  var step = w * 4;
  var r = imagePixels.data[y * step + x * 4];
  var g = imagePixels.data[y * step + x * 4 + 1];
  var b = imagePixels.data[y * step + x * 4 + 2];

  var Y = r * 0.299 + g * 0.587 + b * 0.114;
  var Cr = (r - Y) * 0.713 + 128;
  var Cb = (b - Y) * 0.564 + 128;
  Y = parseInt(Y);
  Cb = parseInt(Math.min(Math.max(Cb, 0), 255));
  Cr = parseInt(Math.min(Math.max(Cr, 0), 255));
  return [Y, Cb, Cr];
};
