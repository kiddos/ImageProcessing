$(document).ready(function() {
  // setup canvas
  initCanvas();
  // highlight the navigation
  $('a[href="/image_processing/lbp"]').addClass('current');
  // set spinner
  $('#spinner').html(
    '<option value="/javascripts/ltp.js">ltp</option>' +
    '<option value="/javascripts/csltp.js">csltp</option>' +
    '<option value="/javascripts/tpltp.js">tpltp</option>'
  );
  // setup editor
  initEditor();
});
