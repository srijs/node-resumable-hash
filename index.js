'use strict';

const stream = require('stream');

const Engine = require('./build/Release/engine');

exports.HashType = {
  0: 'Sha1',
  1: 'Sha256',
  Sha1: 0,
  Sha256: 1
};

class Hash {
  constructor(type, state) {
    this.type = type;
    if (state instanceof Buffer) {
      this._engine = new Engine(type, state);
    } else {
      this._engine = new Engine(type);
    }
  }

  update(data) {
    const hash = new Hash(this.type, this._engine.serialize());
    return new Promise(resolve => {
      hash._engine.update(data, () => resolve(hash));
    });
  }

  updateSync(data) {
    const hash = new Hash(this.type, this._engine.serialize());
    hash._engine.updateSync(data);
    return hash;
  }

  digest() {
    const hash = new Hash(this.type, this._engine.serialize());
    return new Promise(resolve => {
      hash._engine.finalize(digest => resolve(digest));
    });
  }

  digestSync() {
    const hash = new Hash(this.type, this._engine.serialize());
    return hash._engine.finalizeSync();
  }

  serialize() {
    return this._engine.serialize();
  }
}

class HashStream extends stream.Transform {
  constructor(type, state) {
    super();
    this.type = type;
    if (state instanceof Buffer) {
      this._engine = new Engine(type, state);
    } else {
      this._engine = new Engine(type);
    }
  }

  _write(buf, enc, cb) {
    this._engine.update(buf, () => cb());
  }

  _flush(cb) {
    this._engine.finalize(digest => {
      this.push(digest);
      cb();
    });
  }
}

exports.Hash = Hash;
exports.HashStream = HashStream;
