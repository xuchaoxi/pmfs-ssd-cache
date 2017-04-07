/*************************************************************************
	> File Name: nvm-buf-table.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Sat 01 Apr 2017 05:25:00 PM CST
 ************************************************************************/

#ifndef _NVM_BUF_TABLE_H
#define _NVM_BUF_TABLE_H
extern void initNVMBufferTable(size_t size);
extern unsigned long nvmBufferTableHashCode(NVMBufferTag *nvm_buf_tag);
extern size_t nvmBufferTableLookup(NVMBufferTag *nvm_buf_tag, unsigned long hash_code);
extern long nvmBufferTableInsert(NVMBufferTag *nvm_buf_tag ,unsigned long hash_code, size_t nvm_buf_id);
extern long nvmBufferTableDelete(NVMBufferTag *nvm_buf_tag, unsigned long hash_code);
#endif
