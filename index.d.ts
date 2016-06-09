export enum HashType {
  Sha1,
  Sha256
}

export class Hash {
  constructor(type: HashType, init?: Buffer);

  type: HashType;

  update(data: Buffer): Hash;
  digest(): Buffer;

  serialize(): Buffer;
}
