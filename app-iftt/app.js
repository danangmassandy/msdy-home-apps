const express = require('express');
const http = require('http');
const app = express();
const {v4 : uuidv4} = require('uuid')
app.use(express.json());

// for executing command
const { exec } = require("child_process");

if (process.env.NODE_ENV !== 'production') {
    require('dotenv').config();
  }

const port = process.env.PORT || 3000;
const serverAuthKey = process.env.IFTTT_SERVICE_KEY;
const clientMacAddr = process.env.MAC_ADDR;
const feederHost = process.env.FEEDER_HOST;
const feederPort = process.env.FEEDER_PORT;

var pino = require('pino');
var logger = require('pino-http')({
    // Reuse an existing logger instance
    logger: pino(),
    // Define a custom request id function
    genReqId: function (req) { return uuidv4() },
});

app.post('/v1/actions/wake_computer', (req, res) => {
    // log full request + origin ip
    logger(req, res);
    let authKey = req.get('ifttt-service-key');
    if (authKey !== serverAuthKey) {
        // log error auth failed
        req.log.error('Invalid AuthKey!');
        // auth failed, return 401 unauthorized
        res.status(401).header('content-type', 'application/json; charset=utf-8').send(JSON.stringify({
            "errors": [
              {
                "status": "SKIP",
                "message": "Invalid Key!"
              }
            ]
          }));
        return;
    }
    
    // auth okay, execute command
    let currentDate = new Date();
    req.log.info(`Executing command from request triggered at ${currentDate} from IFTTT`);

    if (clientMacAddr) {
        exec(`wakeonlan ${clientMacAddr}`, (error, stdout, stderr) => {
            if (error) {
                req.log.error(`error: ${error.message}`);
                return;
            }
            if (stderr) {
                req.log.error(`stderr: ${stderr}`);
                return;
            }
        });
    } else {
        req.log.warn('Empty client mac address');
    }

    res.status(200).header('content-type', 'application/json; charset=utf-8').send(JSON.stringify({
        "data": [
          {
            "id": req.id
          }
        ]
      }));
});

app.post('/v1/actions/feed_my_cat', (req, res) => {
  // log full request + origin ip
  logger(req, res);
  let authKey = req.get('ifttt-service-key');
  if (authKey !== serverAuthKey) {
      // log error auth failed
      req.log.error('Invalid AuthKey!');
      // auth failed, return 401 unauthorized
      res.status(401).header('content-type', 'application/json; charset=utf-8').send(JSON.stringify({
          "errors": [
            {
              "status": "SKIP",
              "message": "Invalid Key!"
            }
          ]
        }));
      return;
  }
  
  // auth okay, execute command
  let currentDate = new Date();
  req.log.info(`Executing command from request triggered at ${currentDate} from IFTTT`);

  var options = {
    host: feederHost,
    port: feederPort,
    path: '/pet_feeder',
    method: 'GET',
    headers: {'ard_action': 'feed'}
  };

  http.request(options, function(resFeeder) {
    req.log.info('FEEDER STATUS: ' + resFeeder.statusCode);
  }).end();

  res.status(200).header('content-type', 'application/json; charset=utf-8').send(JSON.stringify({
    "data": [
      {
        "id": req.id
      }
    ]
  }));
});

app.get('/v1/status', (req, res) => {
    logger(req, res);
    let authKey = req.get('ifttt-service-key');
    if (authKey !== serverAuthKey) {
        // log error auth failed
        req.log.error('Invalid AuthKey!');
        // auth failed, return 401 unauthorized
        res.status(401).header('content-type', 'application/json; charset=utf-8').end();
        return;
    }

    res.status(200).end();
});

app.post('/v1/test/setup', (req, res) => {
    logger(req, res);
    let authKey = req.get('ifttt-service-key');
    if (authKey !== serverAuthKey) {
        // log error auth failed
        req.log.error('Invalid AuthKey!');
        // auth failed, return 401 unauthorized
        res.status(401).header('content-type', 'application/json; charset=utf-8').send(JSON.stringify({
            "errors": [
              {
                "status": "SKIP",
                "message": "Invalid Key!"
              }
            ]
          }));
        return;
    }

    res.status(200).header('content-type', 'application/json; charset=utf-8').send(JSON.stringify({
        "data": {
            "samples": {
                "actions": {
                    "wake_computer": {}
                }
            }
        }
    }));
});


app.listen(port, () => {
  console.log(`Server running at port ${port}`);
});