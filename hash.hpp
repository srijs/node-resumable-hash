#ifndef HASH_HPP
#define HASH_HPP

class Hash {
public:
  virtual ~Hash();

  size_t stateSize;

  virtual void init();
  virtual void update(uint8_t data[], size_t len);
  virtual uint8_t *finalize(uint32_t *lenptr);

  virtual uint8_t *serialize(uint32_t *lenptr);
  virtual void deserialize(uint8_t data[]);
};

#endif
