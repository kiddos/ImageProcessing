$(document).ready(function() {
  // setup canvas
  initCanvas();
  // highlight the navigation
  $('a[href="/image_processing/lzp"]').addClass('current');
  // set spinner
  $('#spinner').html(
    '<option value="/javascripts/lzp-pattern1.js">lzp pattern 1</option>' +
    '<option value="/javascripts/lzp-pattern2.js">lzp pattern 2</option>' +
    '<option value="/javascripts/tplzp.js">tplzp</option>'
  );
  // setup editor
  initEditor();
});
