$(document).ready(function() {
  // setup canvas
  initCanvas();
  // highlight the navigation
  $('a[href="/image_processing/lbp"]').addClass('current');
  // set spinner
  $('#spinner').html(
    '<option value="/javascripts/ldp-pattern1.js">ldp pattern 1</option>' +
    '<option value="/javascripts/ldp-pattern2.js">ldp pattern 2</option>' +
    '<option value="/javascripts/ldp-pattern3.js">ldp pattern 3</option>' +
    '<option value="/javascripts/ldp-pattern4.js">ldp pattern 4</option>' +
    '<option value="/javascripts/ldp-pattern5.js">ldp pattern 5</option>' +
    '<option value="/javascripts/ldp-pattern6.js">ldp pattern 6</option>' +
    '<option value="/javascripts/ldp-pattern7.js">ldp pattern 7</option>' +
    '<option value="/javascripts/ldp-pattern8.js">ldp pattern 8</option>'
  );
  // setup editor
  initEditor();
});
