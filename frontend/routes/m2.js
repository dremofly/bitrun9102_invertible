var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('m2', { title: 'Express' });
});
router.post('/', function(req, res) {
  
  res.redirect(301, "postvote");
  
})

module.exports = router;
