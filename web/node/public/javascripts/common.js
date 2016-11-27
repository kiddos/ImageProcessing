var inputCanvas, outputCanvas, inputContext, outputContext;
var codeMirror = null;

function initCanvas() {
  inputCanvas = document.getElementById('input');
  outputCanvas = document.getElementById('output');
  inputContext = inputCanvas.getContext('2d');
  outputContext = outputCanvas.getContext('2d');
  inputCanvas.height = inputCanvas.width;
  outputCanvas.width = inputCanvas.width;
  outputCanvas.height = outputCanvas.width;
}

function initEditor() {
  $('#code').load($('#spinner').val(), function() {
    eval($('#code').val());
    if (codeMirror === null) {
      codeMirror = CodeMirror.fromTextArea(
        document.getElementById('code'), {
        lineNumbers: true,
        tabSize: 2,
      });
    }
  });
  $('#spinner').change(function() {
    if (codeMirror !== null) {
      $('#code').load($('#spinner').val(), function() {
        codeMirror.setValue($('#code').val());
      });
    }
  });
}

function convertInputToGray() {
  var w = inputCanvas.width;
  var h = inputCanvas.height;
  var pixels = inputContext.getImageData(0, 0, w, h);
  var step = w * 4;
  for (var i = 0 ; i < inputCanvas.height ; ++i) {
    for (var j = 0 ; j < inputCanvas.width ; ++j) {
      var r = pixels.data[i * step + j * 4];
      var g = pixels.data[i * step + j * 4 + 1];
      var b = pixels.data[i * step + j * 4 + 2];
      var outputVal =  r * 0.299 + g * 0.581 + b * 0.114;
      for (var k = 0 ; k < 3 ; ++k) {
        pixels.data[i * step + j * 4 + k] = outputVal;
      }
      pixels.data[i * step + j * 4 + 3] = 255;
    }
  }
  inputContext.putImageData(pixels, 0, 0);
}
