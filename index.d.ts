export enum HashType {
  Sha1,
  Sha256
}

export class Hash {
  constructor(type: HashType, init?: Buffer);

  type: HashType;

  update(data: Buffer): Promise<Hash>;
  digest(): Promise<Buffer>;

  updateSync(data: Buffer): Hash;
  digestSync(): Buffer;

  serialize(): Buffer;
}

export class MutableHash {
  constructor(type: HashType, init?: Buffer);

  type: HashType;

  update(data: Buffer): Promise<void>;
  digest(): Promise<Buffer>;

  updateSync(data: Buffer);
  digestSync(): Buffer;

  serialize(): Buffer;
}
