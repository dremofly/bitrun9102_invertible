var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('tt', { title: 'tt' });
});
router.post('/', function(req, res) {
  
  res.redirect(301, "signup");
  
})

module.exports = router;
