const http = require('http');
// if (process.env.NODE_ENV !== 'production') {
//     require('dotenv').config();
//   }

const feederHost = '192.168.7.7';
const feederPort = 8080;

// auth okay, execute command
let currentDate = new Date();
console.log(`Executing command feeder from scheduler triggered at ${currentDate}`);

var options = {
  host: feederHost,
  port: feederPort,
  path: '/pet_feeder',
  method: 'GET',
  headers: {'ard_action': 'feed'}
};

http.request(options, function(resFeeder) {
  console.log('FEEDER STATUS: ' + resFeeder.statusCode);
}).end();

// command to run
//  pm2 start feed_scheduler.js --no-autorestart --instances 1 --cron "0 */4 * * *"