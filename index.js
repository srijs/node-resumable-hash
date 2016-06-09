'use strict';

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

  digest() {
    const hash = new Hash(this.type, this._engine.serialize());
    return new Promise(resolve => {
      hash._engine.finalize(digest => resolve(digest));
    });
  }

  serialize() {
    return this._engine.serialize();
  }
}

exports.Hash = Hash;
