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
    res.end(hasher.read());
  });
  input.pipe(hasher);
});

app.get('/async', (req, res) => {
  const hasher = new hash.HashStream(hash.HashType.Sha1);
  const input = fs.createReadStream('./random');
  hasher.on('readable', () => {
    res.end(hasher.read());
  });
  input.on('data', (buf) => console.log(buf.length)).pipe(hasher);
});

app.listen(12345);
