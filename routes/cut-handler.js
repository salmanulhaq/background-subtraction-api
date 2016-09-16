var router = require('express').Router();
var fs = require('fs');
var bgsub = require('/usr/lib/bgsub');

router.get('/', function(request, response, next) {

	var params = request.query;

	var numThreads = params.num_threads || 2;
	
	if (!params.url || params.url == '') {
		response.status(501).json({status:'ERROR', message: 'You must specify "url" param'});
		return next();
	}
	
	bgsub.subtract(params.url, function(error, contentLength, outputImage){
		if(error) {
			response.status(501).json({status:'ERROR', message: error});
			return next();
		} else {
			response.header('Content-Type', 'image/png');
            response.header('Content-Length', contentLength);
			response.status(200).send(outputImage);
            return next();
		}
	});
});

module.exports = router;