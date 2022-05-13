const http = require('http');
const express = require('express');
const app = express();
app.use(express.json());

if (process.env.NODE_ENV !== 'production') {
    require('dotenv').config();
  }

const feederHost = process.env.FEEDER_HOST;
const feederPort = process.env.FEEDER_PORT;
// dummy port
const port = 3100;

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

app.listen(port, () => {
    console.log(`Dummy Server running at port ${port}`);
  });