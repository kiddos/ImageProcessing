var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.redirect('/image_processing');
});

module.exports = router;
