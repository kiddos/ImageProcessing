var express = require('express');
var router = express.Router();

/* GET image processing page. */
router.get('/', function(req, res, next) {
  res.render('image_processing', { title: 'Image Processing' });
});

router.get('/rgb2color', function(req, res, next) {
  res.render('rgb2color', { title: 'RGB to Different Color Space' });
});

router.get('/lbp', function(req, res, next) {
  res.render('lbp', { title: 'Local Binary Pattern (LBP)' });
});

router.get('/ltp', function(req, res, next) {
  res.render('ltp', { title: 'Local Ternary Pattern (LTP)' });
});

router.get('/lzp', function(req, res, next) {
  res.render('lzp', { title: 'Local Zigzag Pattern (LZP)' });
});

router.get('/ldp', function(req, res, next) {
  res.render('ldp', { title: 'Local Direction Pattern (LDP)' });
});

router.get('/sobel', function(req, res, next) {
  res.render('sobel', { title: 'Sobel Derivative' });
});

router.get('/canny', function(req, res, next) {
  res.render('canny', { title: 'Canny Edge Detection' });
});

router.get('/laplacian', function(req, res, next) {
  res.render('laplacian', { title: 'Laplacian' });
});

module.exports = router;
