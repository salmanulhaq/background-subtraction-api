#!sh
echo "Starting installation of image background subtraction module"
sudo apt-get update
sudo apt-get install -y build-essential libopencv-dev libcurl4-gnutls-dev nodejs npm
cd node-bgsub && sudo cp bgsub.pc /usr/lib
npm install
cd -
npm install
clear
echo "Installed! starting server which would be accessible at localhost|ip_address:4000/cut?url=image_url"
node server.js