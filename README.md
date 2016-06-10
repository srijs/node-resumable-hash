# Resumable Hash

> A functional, native implementation of resumable SHA-1 and SHA-256

## Installation

```
npm install resumable-hash
```

## Usage

```
import {Hash} from 'resumable-hash';
```

### new Hash(type, [init])

Constructs a hash of a certain type, with an optional initialisation payload.

### Hash#update(data)

Updates the hash with the given data buffer. Does not modify the current hash, but instead returns a new hash.

### Hash#digest()

Computes the digest of the hash. Returns a buffer that can be converted to hexadecimal, etc. using `Buffer#toString()`. Does not modify the original hash.

### Hash#serialize()

Serializes the hash state into a buffer and returns it. The returned buffer can be used in another call to `new Hash` to resume the hashing.

Note: The serialized state is endian-specific, and should only be transferred between machines of the same endianess.