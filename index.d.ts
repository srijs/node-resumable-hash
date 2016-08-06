export enum HashType {
  Sha1,
  Sha256
}

export class Hash {
  constructor(type: HashType, init?: Buffer);

  type: HashType;

  update(data: Buffer): Promise<Hash>;
  digest(): Promise<Buffer>;

  updateSync(): Hash;
  digestSync(): Buffer;

  serialize(): Buffer;
}
