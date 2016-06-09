var hash = require('./index');

var assert = require('assert');
var crypto = require('crypto');

var type = 'sha256';
var hasher = hash.createEngine(type);

var random1 = crypto.randomBytes(Math.floor(Math.random() * 1024 * 1024));
var random2 = crypto.randomBytes(Math.floor(Math.random() * 1024 * 1024));

var hash1 = crypto.createHash(type)
  .update(random1)
  .update(random2)
  .digest('hex');
console.log(hash1);

hasher.update(random1, () => {
  var state = hasher.serialize();
  var hasher2 = hash.createEngine(type, state);
  hasher2.update(random2, () => {
    hasher2.finalize((digest) => {
      console.log(digest.toString('hex'));
    });
  });
});
