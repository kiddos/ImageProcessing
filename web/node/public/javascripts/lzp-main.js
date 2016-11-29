$(document).ready(function() {
  // setup canvas
  initCanvas();
  // highlight the navigation
  $('a[href="/image_processing/lbp"]').addClass('current');
  // set spinner
  $('#spinner').html(
    '<option value="/javascripts/lzp-pattern1.js">lzp pattern 1</option>' +
    '<option value="/javascripts/lzp-pattern2">lzp pattern 2</option>' +
    '<option value="/javascripts/lzp-pattern3.js">lzp pattern 3</option>'
  );
  // setup editor
  initEditor();
});
