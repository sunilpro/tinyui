const http2 = require('http2')
var express = require("express")
var compression = require("shrink-ray-current")
 
var app = express();

app.use(compression({filter: shouldCompress}))

function shouldCompress(req, res) {
  //console.log(req.headers['accept-encoding'])
  if (req.headers['accept-encoding'].includes("gzip")) {
    return true
  }

  // fallback to standard filter function
  return compression.filter(req, res)
}

express.static.mime.types['data'] = 'multipart/bag'
express.static.mime.types['wasm'] = 'application/wasm' 
app.use(express.static('./'));
 
//make way for some custom css, js and images
//app.use('/css', express.static(__dirname + '/public/css'));
//app.use('/js', express.static(__dirname + '/public/js'));
//app.use('/images', express.static(__dirname + '/public/images'));
 
var server = app.listen(process.env.PORT || 8081, function(){
    var port = server.address().port;
    console.log("Server started at http://localhost:%s", port);
});
