var Engine = require('./build/Release/engine');

exports.createEngine = function (type, state) {
  var typeCode;
  switch (type) {
    case 'sha1': typeCode = 0; break;
    case 'sha256': typeCode = 1; break;
    default: throw new Error('unknown hash type: ' + type);
  }
  if (state instanceof Buffer) {
    return new Engine(typeCode, state);
  }
  return new Engine(typeCode);
};
