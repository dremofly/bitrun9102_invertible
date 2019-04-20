App = {
  sub: function() {
    var fs = require("fs");
    var filespec="/Users/hong/Downloads/res.txt";
    //var s=filespec;   
    fs.readFile(filespec, function (err, data) {
        if (err) {
            return console.error(err);
        }
        return console.log("异步读取: " + data.toString());
    });
    },  
  
    jumpvote: function() {
      return window.location.href = "Post_Vote.html";
    }
 };