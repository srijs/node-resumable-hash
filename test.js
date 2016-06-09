var hash = require('./index');

var assert = require('assert');
var crypto = require('crypto');

var type = 'Sha256';
var hasher = new hash.Hash(hash.HashType[type]);

var random1 = crypto.randomBytes(Math.floor(Math.random() * 1024 * 1024));
var random2 = crypto.randomBytes(Math.floor(Math.random() * 1024 * 1024));

var hash1 = crypto.createHash(type.toLowerCase())
  .update(random1)
  .update(random2)
  .digest('hex');
console.log(hash1);

hasher.update(random1).then(hasher2 => {
  return hasher2.update(random2).then(hasher3 => {
    return hasher3.digest();
  });
}).then(res => console.log(res.toString('hex')), err => console.log(err));
