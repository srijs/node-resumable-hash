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
  const buf = Buffer.alloc(len);
  for (let i = 0; i < len; i++) {
    buf[i] = (gen() * 255) << 0;
  }
  return buf;
}

function testHashingWithTwoBuffersAsync(gen, type) {
  const random1 = randomBuffer(gen);
  const random2 = randomBuffer(gen);

  const hash1 = crypto
    .createHash(hash.HashType[type].toLowerCase())
    .update(random1)
    .update(random2)
    .digest('hex');

  const hasher = new hash.Hash(type);

  return hasher.update(random1).then(hasher2 => {
    return hasher2.update(random2).then(hasher3 => {
      return hasher3.digest().then(digest => {
        assert.equal(digest.toString('hex'), hash1);
      });
    });
  });
}

function testHashingWithTwoBuffersSync(gen, type) {
  const random1 = randomBuffer(gen);
  const random2 = randomBuffer(gen);

  const hash1 = crypto
    .createHash(hash.HashType[type].toLowerCase())
    .update(random1)
    .update(random2)
    .digest('hex');

  const hasher = new hash.Hash(type);

  const hasher2 = hasher.updateSync(random1);
  const hasher3 = hasher2.updateSync(random2);
  assert.equal(hasher3.digestSync().toString('hex'), hash1);
}

const seed = crypto.randomBytes(8).toString('hex');
const gen = random(seed);

console.log('seed:', seed);

describe('Hash', () => {
  it('throws when constructed with an invalid hash type', () => {
    assert.throws(() => new hash.Hash(3), /Unknown hash type/);
  });

  describe('Sha1', () => {
    it('throws when initialised with invalid payload', () => {
      assert.throws(
        () => new hash.Hash(hash.HashType.Sha1, Buffer.from('foo')),
        /Invalid init payload/,
      );
    });

    it('throws when updated with an invalid payload', () => {
      assert.throws(() => {
        const h = new hash.Hash(hash.HashType.Sha1);
        h.updateSync('foo');
      }, /Argument should be a buffer object/);
    });

    it('can init from a previous payload', () => {
      const initPayload = Buffer.from(
        [
          '67452301efcdab8998badcfe10325476c3d2e1f0000001f80000000062e6644',
          '60dca9f8bfeb17084a24e3f396076a677d12a302552738d9d6b44a12fd9a68f',
          'deb8ad5eb3b46f105ef757a78c976b69792abeeb19a8a1f0e3005ab0600000003f',
        ].join(''),
        'hex',
      );
      const updatePayload = Buffer.from(
        [
          '0f2ebec03321dd1b14a7ff9cd19d1898857cf217357d0e11e9daeb960e731db',
          '7df0a737ecc9fdb7c3a6ee326c66443cb3374a92c06c4e547088af0acbda23c',
        ].join(''),
        'hex',
      );
      const h = new hash.Hash(hash.HashType.Sha1, initPayload)
        .updateSync(updatePayload)
        .digestSync();
      assert.equal(h.toString('hex'), '319cff4ee8d4d0418ec21bac5b085ffa4e080206');
    });

    it('works with two buffers (async)', () => {
      return repeat(50, () => testHashingWithTwoBuffersAsync(gen, hash.HashType.Sha1));
    }).timeout(10000);

    it('works with two buffers (sync)', () => {
      for (let i = 0; i < 50; i++) {
        testHashingWithTwoBuffersSync(gen, hash.HashType.Sha1);
      }
    }).timeout(10000);
  });

  describe('Sha256', () => {
    it('throws when initialised with invalid payload', () => {
      assert.throws(
        () => new hash.Hash(hash.HashType.Sha256, Buffer.from('foo')),
        /Invalid init payload/,
      );
    });

    it('throws when updated with an invalid payload', () => {
      assert.throws(() => {
        const h = new hash.Hash(hash.HashType.Sha256);
        h.updateSync('foo');
      }, /Argument should be a buffer object/);
    });

    it('can init from a previous payload', () => {
      const initPayload = Buffer.from(
        [
          '6a09e667bb67ae853c6ef372a54ff53a510e527f9b05688c1f83d9ab5be0cd1',
          '9000001f8000000008bab53c65a7693df652a683b434b43fe5f9d9f2a4274f8',
          'c36161cfebe0de810843bc37d08e20c318bedead84dc69479b9afc8fb67ba65',
          '552c4c89df200db15a10000003f00000020',
        ].join(''),
        'hex',
      );
      const updatePayload = Buffer.from(
        [
          'dc24f55baaf0da39ac6fd6208eb686f997804c4cb107126c62f8cd34549a6f7fb',
          '7a86cea8786eec2487fa684e90af196a73d7098201abb34d4f1dcfdb35fc64e49',
        ].join(''),
        'hex',
      );
      const h = new hash.Hash(hash.HashType.Sha256, initPayload)
        .updateSync(updatePayload)
        .digestSync();
      assert.equal(
        h.toString('hex'),
        '8437298b6841d3cd63cbda2fed793b5412259e138c874a28eed61dc007c207aa',
      );
    });

    it('works with two buffers (async)', () => {
      return repeat(50, () => testHashingWithTwoBuffersAsync(gen, hash.HashType.Sha256));
    }).timeout(10000);

    it('works with two buffers (sync)', () => {
      for (let i = 0; i < 50; i++) {
        testHashingWithTwoBuffersSync(gen, hash.HashType.Sha256);
      }
    }).timeout(10000);
  });
});
