var codeMirror = null;

$(document).ready(function() {
  // setup canvas
  initCanvas();
  // highlight the navigation
  $('a[href="/image_processing/rgb2color"]').addClass('current');
  // set spinner
  $('#spinner').html(
    '<option value="/javascripts/rgb2gray.js">RGB to Gray</option>' +
    '<option value="/javascripts/rgb2gray.js">RGB to HUV</option>' +
    '<option value="/javascripts/rgb2gray.js">RGB to YCbCr</option>'
  );
  // setup editor
  initEditor();
});

$('#upload').on('change', function(event) {
  var image = new Image();
  image.src = URL.createObjectURL(event.target.files[0]);
  image.onload = function() {
    inputContext.drawImage(image, 0, 0, image.width, image.height,
      0, 0, inputCanvas.width, inputCanvas.height);
  };
});

$('#process').on('click', function() {
  eval(codeMirror.getValue());
  var w = inputCanvas.width;
  var h = inputCanvas.height;
  var pixels = inputContext.getImageData(0, 0, w, h);
  var output = outputContext.getImageData(0, 0, w, h);
  var step = w * 4;
  for (var i = 0 ; i < inputCanvas.height ; ++i) {
    for (var j = 0 ; j < inputCanvas.width ; ++j) {
      var outputVal = kernel(j, i, w, h, pixels);
      for (var k = 0 ; k < 3 ; ++k) {
        output.data[i * step + j * 4 + k] = outputVal[k];
      }
      output.data[i * step + j * 4 + 3] = 255;
    }
  }
  outputContext.putImageData(output, 0, 0);
});
