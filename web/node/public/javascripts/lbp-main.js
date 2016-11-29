$(document).ready(function() {
  // setup canvas
  initCanvas();
  // highlight the navigation
  $('a[href="/image_processing/lbp"]').addClass('current');
  // set spinner
  $('#spinner').html(
    '<option value="/javascripts/lbp.js">lbp</option>' +
    '<option value="/javascripts/tlbp.js">tlbp</option>' +
    '<option value="/javascripts/clbp.js">clbp</option>' +
    '<option value="/javascripts/tplbp.js">tplbp</option>'
  );
  // setup editor
  initEditor();
});
