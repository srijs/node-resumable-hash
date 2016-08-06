# Resumable Hash [![CircleCI](https://circleci.com/gh/srijs/node-resumable-hash.svg?style=svg)](https://circleci.com/gh/srijs/node-resumable-hash)

> A functional, native implementation of resumable SHA-1 and SHA-256

## Disclaimer

This is not a cryptographic hash implementation. It does not prevent against side-channel attacks, et al. If you require a cryptographic hash function, use the Node.js `crypto` module instead.

## Installation

```
npm install resumable-hash
```

## Usage

```
import {Hash, HashType} from 'resumable-hash';
```

### `new Hash(type, [init])`

Constructs a hash of a certain type, with an optional initialisation payload.

#### Available types:

- `HashType.Sha1`
- `HashType.Sha256`

### `Hash#update(data): Promise<Hash>`

Updates the hash with the given data buffer. Does not modify the current hash, but instead returns a new hash.

### `Hash#updateSync(data): Hash`

Updates the hash with the given data buffer. Does not modify the current hash, but instead returns a new hash.

### `Hash#digest(): Promise<Buffer>`

Computes the digest of the hash. Returns a buffer that can be converted to hexadecimal, etc. using `Buffer#toString()`. Does not modify the original hash.

### `Hash#digestSync(): Buffer`

Computes the digest of the hash. Returns a buffer that can be converted to hexadecimal, etc. using `Buffer#toString()`. Does not modify the original hash.

### `Hash#serialize(): Buffer`

Serializes the hash state into a buffer and returns it. The returned buffer can be used in another call to `new Hash` to resume the hashing.

Note: The serialized state is endian-specific, and should only be transferred between machines of the same endianess.

## Performance

`resumable-hash` has a fully asynchronous API and performs hashing in a background thread to not block the main event loop.
It is built for cases where a server performs stream hashing as part of answering requests, but also serves other requests which don't require hashing. With the built-in Node.js hashing, the hashing requests prevent the server from answering all other requests, driving up latency.

To simulate this scenario, a synthetic server was created which supports three endpoints: `/` which just serves a simple JSON response body, and `/sync` and `/async` which perform synchronous hashing (via the built-in `crypto` module) and asynchronous hashing (via `resumable-hash`), respectively.

As a benchmark, both `/` and `/sync` and `/` and `/async` were hit repeatedly and the latency for the `/` endpoint was measured. Below are the results, performed on a 2,3 GHz Intel Core i7 MacBook Pro:

**Synchronous hashing (built-in `crypto`):**

```
Running 30s test @ http://localhost:12345/
  2 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    20.69ms    4.11ms  51.36ms   95.61%
    Req/Sec   242.66     20.28   303.00     79.70%
  14550 requests in 30.10s, 1.85MB read
Requests/sec:    483.37
Transfer/sec:     62.78KB
```

**Asynchronous hashing (`resumable-hash`):**

```
Running 30s test @ http://localhost:12345/
  2 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     1.07ms  673.30us  22.89ms   93.31%
    Req/Sec     4.89k   693.44    10.41k    74.54%
  292541 requests in 30.10s, 37.11MB read
Requests/sec:   9717.74
Transfer/sec:      1.23MB
```
