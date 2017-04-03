/*************************************************************************
	> File Name: nvm-buf-table.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:25:00 PM CST
 ************************************************************************/

#ifndef _NVM-BUF-TABLE_H
#define _NVM-BUF-TABLE_H
extern void initNVMBufferTable(size_t size);
extern unsigned long nvmBufferTableHashcode(off_t nvm_buf_tag);
extern size_t nvmBufferTableLookup(off_t offset, unsigned long hash_code);
extern long nvmBufferTableInsert(off_t ,unsigned long hash_code, size_t nvm_buf_id);
extern long nvmBufferTableDelete(off_t, unsigned long hash_code);
#endif
