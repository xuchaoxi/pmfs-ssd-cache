# pmfs-ssd-cache
* main.c
    - open device
    - trace_to_call
* trace_to_call.c
    - write_block
* nvm_cache.c
    - initNVMBuffer -> initNVMStrategyBuffer & initBufferTable
    - flushNVMBuffer -> pread from NVM -> write to SSD
    - NVMBufferAlloc -> hit(return this NVM Buffer) -> not hit(get a new NVM buffer, insert into NVMBufferTable)
    - initStrategyNVMBuffer -> initNVMBufferForLRU
    - getStrategyNVMBuffer -> return LRUBuffer
    - hitInNVMBuffer -> hit in LRU Buffer
    - read_block  write_block
* nvm-cache.h
    - NVMBufferTag -> offset(offset user request for, global_raid_address)
    - NVMBufferDesc -> NVMBufferTag,nvm_buf_id, nvm_buf_flag,next
    - NVMBufferHashBucket -> NVMBufferHashTables
    - NVMBufferControl -> u_usednvm, first_freenvm, last_freenvm;
* nvm-buf-table
    - initNVMBufferTable -> initialize NVMBufferHashBucket
    - nvmBufferTableHashCode -> get hash code from offset
    - nvmBufferTableLookup -> look for table from NVMBufferHashBucket
    - nvmBufferTableInsert -> insert into NVMBufferHashBucket
    - nvmBufferTableDelete -> delete from NVMBufferHashBucket
* lru.c
    - initNVMBufferForLRU -> initialize NVMBuffer for LRU
    - addToLRUHead -> add buffer to NVMBuffer for LRU head
    - deleteFromLRU -> delete buffer from NVMBuffer For LRU 
    - moveToLRUHead -> move buffer to NVMBuffer for LRU head
    - getLRUBuffer -> get buffer from NVMBuffer for LRU 
    - hitInLRUBuffer -> hit NVMBuffer for LRU

