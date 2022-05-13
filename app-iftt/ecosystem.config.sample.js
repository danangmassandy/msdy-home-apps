module.exports = {
    apps : [{
      name: 'app-iftt',
      script: './app.js',
      env: {
          NODE_ENV: 'dev',
          PORT: 3000,
          IFTTT_SERVICE_KEY: '',
          MAC_ADDR: '',
          FEEDER_HOST: '',
          FEEDER_PORT: ''
      }
    },
    {
        name: 'CRON',
        script: "./feed_scheduler.js",
        instances: 1,
        exec_mode: 'fork',
        cron_restart: "0 */4 * * *",
        watch: false,
        autorestart: false,
        env: {
            NODE_ENV: 'production',
            FEEDER_HOST: '',
            FEEDER_PORT: ''
        }
    }]
  };