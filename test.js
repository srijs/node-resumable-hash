'use strict';

const hash = require('./index');

const assert = require('assert');
const crypto = require('crypto');

const random = require('seed-random');

function repeat(n, action) {
  if (n === 0) {
    return Promise.resolve();
  }
  return action().then(() => repeat(n - 1, action));
}

function randomBuffer(gen) {
  const len = gen() * 1024 * 1024;
  const buf = new Buffer(len);
  for (let i = 0; i < len; i++) {
    buf[i] = (gen() * 255) << 0;
  }
  return buf;
}

function testHashingWithTwoBuffers(gen, type) {
  const random1 = randomBuffer(gen);
  const random2 = randomBuffer(gen);

  const hash1 = crypto.createHash(hash.HashType[type].toLowerCase())
    .update(random1)
    .update(random2)
    .digest('hex');

  const hasher = new hash.Hash(type);

  return hasher.update(random1).then(hasher2 => {
    return hasher2.update(random2).then(hasher3 => {
      return hasher3.digest().then(digest => {
        assert.equal(digest.toString('hex'), hash1);
      })
    });
  });
}

const seed = crypto.randomBytes(8).toString('hex');
const gen = random(seed);

console.log('seed:', seed);

describe('Hash', () => {

  describe('Sha1', () => {

    it('works with two buffers', () => {
      return repeat(50, () => testHashingWithTwoBuffers(gen, hash.HashType.Sha1));
    }).timeout(5000);

  });

  describe('Sha256', () => {

    it('works with two buffers', () => {
      return repeat(50, () => testHashingWithTwoBuffers(gen, hash.HashType.Sha256));
    }).timeout(5000);

  });

});
