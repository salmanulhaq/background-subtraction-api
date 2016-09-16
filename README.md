#Background Subtraction
Image background subtraction API provides a complete API which can be used to remove backgrounds from images. The process is automatic and does not require any manual input e.g. selecting pixel regions from the foreground or background or cold start etc.

Here's sample input and output images:

<img src="http://i.imgur.com/mBCE87y.jpg" width=300>
<img src="http://i.imgur.com/2V5SQEa.png" width=300>

#Installation
There's a *install.sh* script included, just run it on a vanilla Ubuntu 14.04 and it would install all pre-requisites and build the code and start the NodeJS API server and start listening on port 4000.

##Usage
Once the code is deployed, you can start using the API as:

```js
http://localhost:4000/cut?url=http://i.imgur.com/mBCE87y.jpg```