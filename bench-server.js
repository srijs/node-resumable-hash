'use strict';

const hash = require('./index');

const fs = require('fs');
const crypto = require('crypto');

const express = require('express');

const app = express();

app.get('/', (req, res) => {
  res.end(JSON.stringify({
    foo: 42
  }));
})

app.get('/sync', (req, res) => {
  const hasher = crypto.createHash('sha1');
  const input = fs.createReadStream('./random');
  hasher.on('readable', () => {
    const read = hasher.read();
    if (read) {
      res.write(read.toString('hex'));
    } else {
      res.end();
    }
  });
  input.pipe(hasher);
});

app.get('/async', (req, res) => {
  const hasher = new hash.HashStream(hash.HashType.Sha1);
  const input = fs.createReadStream('./random');
  hasher.on('readable', () => {
    const read = hasher.read();
    if (read) {
      res.write(read.toString('hex'));
    } else {
      res.end();
    }
  });
  input.pipe(hasher);
});

app.listen(12345);
