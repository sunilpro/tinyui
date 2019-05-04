var express = require("express");
 
var app = express();

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
