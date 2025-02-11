/* @file: readwrite.h
 *
 * @breif: Macros and data strutures for readwrite.c
 *
 * @author: Yuhang Jiang (yuhangj@andrew.cmu.edu)
 * @bug: No known bugs
 */

#ifndef _READWRITE_H_
#define _READWRITE_H_

void print_sector (unsigned char *buf);
void read_sectors (int64_t start_sector, ssize_t len, void *into);
void write_sectors (int64_t start_sector, unsigned int num_sectors, void *from);

void read_bytes(int64_t base, ssize_t buf_len, void* into);
void write_bytes(int64_t base, ssize_t buf_len, void* from);

#endif /* !_READWRITE_H_ */
