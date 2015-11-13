#ifndef MULTIVERSO_ARENA_H_
#define MULTIVERSO_ARENA_H_

#include "base.h"

namespace multiverso {

// Arena is a continuous chunk of memory. It is designed for the frequent 
// message allocation in multiverso. Arena records the average memory size
// and total size requested by custmor between two Arena::Reset() calls. And 
// arena adjust the initial block size as half of total memory and 
// block unit size as 2 * ave(size) when Reset() for next time's uses.
//
// Arena is not thread-safe and is designed to use within one thread.
class Arena {
 public:
  Arena();
  ~Arena();

  // Allocate a block memory
  // TODO(feiga): consider memory alignment
  char* Allocate(size_t bytes);

  void Reset();

  std::string DebugString() const;

 private:
  struct MemBlock {
    char* memory;
    size_t size;
  };

  // The initial memory is relatively big. The best case is this memory
  // is enough for use, as well not too big to waste. 
  MemBlock initial_mem_; 

  // After initial_mem_ is exhausted, we allocate a series of default-size
  // block for further use.
  std::vector<MemBlock> mem_blocks_;

  char* next_ptr_;
  size_t available_size_;

  size_t total_size_;      // total size arena holds
  size_t allocated_size_;  // total size allocated

  size_t request_size_;    // sum of requested size 
  size_t reqeust_time_;    // time of request

  size_t initial_block_size_;
  size_t block_unit_size_;

  MemBlock AllocateNew(size_t bytes);
  void FreeBlocks();
  void Resize();

  DISALLOW_COPY_AND_ASSGIN(Arena);
};

inline char* Arena::Allocate(size_t bytes) {
  // TODO
}

} // namespace multiverso

#endif // MULTIVERSO_ARENA_H_
