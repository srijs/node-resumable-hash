'use strict';

const hash = require('./index');

const crypto = require('crypto');

const express = require('express');

const app = express();

const buffer = new Buffer(4 * 1024 * 1024);
const hasher = new hash.Hash(hash.HashType.Sha1);

app.get('/', (req, res) => {
  res.end(JSON.stringify({
    foo: 42
  }));
})

app.get('/sync', (req, res) => {
  res.end(crypto.createHash('sha1').update(buffer).digest());
});

app.get('/async', (req, res) => {
  hasher.update(buffer).then(hasher2 => hasher2.digest()).then(digest => {
    res.end(digest);
  });
});

app.listen(12345);
