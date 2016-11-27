var kernel = function(x, y, w, h, imagePixels) {
  var step = w * 4;
  var r = imagePixels.data[y * step + x * 4];
  var g = imagePixels.data[y * step + x * 4 + 1];
  var b = imagePixels.data[y * step + x * 4 + 2];
  var gray = parseInt(r * 0.299 +
    g * 0.587 + b * 0.114);
  gray = Math.min(255, Math.max(gray, 0));
  return [gray, gray, gray];
};
