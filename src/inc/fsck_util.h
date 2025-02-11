/* @file: fsck_util.h
 *
 * @breif: Macros and data strutures for myfsck
 *
 * @author: Yuhang Jiang (yuhangj@andrew.cmu.edu)
 * @bug: No known bugs
 */

#ifndef _FSCK_UTIL_H_
#define _FSCK_UTIL_H_

#include "partition.h"
#include "ext2_fs.h"

#define SECT_SIZE		512

/** Macros for MBR */
#define PT_E_NUM		4
#define TYPE_OFFSET		0x04
#define SECT_OFFSET 	0x08
#define LEN_OFFSET		0x0c

/** Macros for directory entry */
#define REC_LEN_OFFSET		0x04
#define NAME_LEN_OFFSET		0x06
#define FILE_TYPE_OFFSET	0x07
#define NAME_OFFSET 		0x08

#define PARSE_SUCC	0
#define PARSE_FAIL	-1

#define INIT_SUCC	0
#define INIT_FAIL	-1

#define FIX_SUCC	0
#define FIX_FAIL	-1

#define EXT2_ISSOCK(m) (((m)&(0xf000)) == (EXT2_S_IFSOCK))
#define EXT2_ISLNK(m) (((m)&(0xf000)) == (EXT2_S_IFLNK))
#define EXT2_ISREG(m) (((m)&(0xf000)) == (EXT2_S_IFREG))
#define EXT2_ISBLK(m) (((m)&(0xf000)) == (EXT2_S_IFBLK))
#define EXT2_ISDIR(m) (((m)&(0xf000)) == (EXT2_S_IFDIR))
#define EXT2_ISCHR(m) (((m)&(0xf000)) == (EXT2_S_IFCHR))
#define EXT2_ISFIFO(m) (((m)&(0xf000)) == (EXT2_S_IFIFO))

/** Macros for fsck information */
typedef struct fsck_info{
	sblock_t sblock;
	partition_t pt;
	grp_desc_t *blkgrp_desc_tb;
	int* inode_map;
	int* block_map;
	uint8_t* bitmap;
}fsck_info_t;

/** Functions and helper function for parsing partition information */
int parse_pt_info(partition_t *pt, uint32_t pt_num);
void print_pt_info(partition_t *pt);

/** Function for parsing super-block information */
int parse_sblock(fsck_info_t *fsck_info, uint32_t pt_num);

/** Function for parsing block group descriptor table information */
int parse_blkgrp_desc_tb(fsck_info_t *fsck_info, uint32_t pt_num);

/** Helper functions for myfsck */
int fsck_info_init(fsck_info_t *fsck_info, uint32_t pt_num);
void destroy_fsck_info(fsck_info_t *fsck_info);
void clear_local_inode_map(fsck_info_t *fsck_info);
int do_fix(int fix_pt_num);

int get_block_size(sblock_t *sblock);
int get_inode_size(sblock_t *sblock);
int get_blocks_num(sblock_t *sblock);
int get_blks_per_group(sblock_t *sblock);
int get_inodes_num(sblock_t *sblock);
int get_inds_per_group(sblock_t *sblock);
int get_groups_num(sblock_t *sblock);

void debug_sblock(fsck_info_t *fsck_info);
void dump_grp_desc(grp_desc_t *entry, int num);
int compute_inode_addr(fsck_info_t *fsck_info, uint32_t inode_num);

#endif /* !_FSCK_UTIL_H_ */
