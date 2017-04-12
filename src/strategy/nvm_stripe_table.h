/*************************************************************************
	> File Name: nvm_stripe_table.h
	> Author: Chaoxi Xu
	> Mail: xuchaoxikb@gmail.com
	> Created Time: Tue 11 Apr 2017 08:34:08 PM CST
 ************************************************************************/

#ifndef _NVM_STRIPE_TABLE_H
#define _NVM_STRIPE_TABLE_H

extern void initNVMStripeTable(size_t size);
extern unsigned long nvmStripeTableHashcode(unsigned long stripe_id);
extern long nvmStripeTableLookup(unsigned long stripe_id, unsigned long hashcode);
extern long nvmStripeTableInsert(unsigned long stripe_id, unsigned long hashcode, long lru_buf_id);
extern long nvmStripeTableDelete(unsigned long stripe_id, unsigned long hashcode);
extern unsigned long nvmStripeTableHashCode(unsigned long stripe_id);

#endif
