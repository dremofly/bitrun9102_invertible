var express = require('express');
var router = express.Router();
var path = require("path");
var fs = require("fs");

//let filePath = path.join(__dirname, "..", "leavemsg");

function run(filePath) {
    let str;
    fs.readFile(filePath,{encoding:"utf-8"}, function (err, fr) {
           //readFile回调函数
           if (err) {
             console.log(err);
            }else {
             str = fr;
             //console.log(str);
            }
        });
    return str;
}

/* GET home page. */

router.get('/', function(req, res, next) {
  res.render('signup', { title: 'Express' });
});

router.post('/', function(req, res) {
  let path = '/Users/hong/Downloads/res.txt';
  var data = fs.readFileSync(path);
  console.log("同步读取: " + data.toString());
  
  if(data==0) {
    res.render('m1', {title: 'Message'});
  } else {
    res.render('m2', {title: 'Message'});
  }
  
  
})

module.exports = router;
