/* @file: pass3.c
 *
 * @breif: Functions to validate pass3
 *
 * @author: Yuhang Jiang (yuhangj@andrew.cmu.edu)
 * @bug: No known bugs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#include "fsck_util.h"
#include "partition.h"
#include "readwrite.h"
#include "ext2_fs.h"
#include "pass1.h"

/** @brief  pass3 - fix the link count
 *  
 *  @param  fsck_info: the ptr to the structure of fsck_info
 *  @return void
 */
void pass3_fix_link_count(fsck_info_t *fsck_info)
{
	/* Clear the local inode map and correct the directory after put 
	 * into lost+found */
	clear_local_inode_map(fsck_info);
	trav_dir(fsck_info, EXT2_ROOT_INO, EXT2_ROOT_INO);

	int inode_addr;
	inode_t inode;
	int i = 1;
	int inodes_num = get_inodes_num(&fsck_info->sblock);
	
	for(; i < inodes_num; i++)
	{
		inode_addr = compute_inode_addr(fsck_info, i);

		read_bytes(inode_addr,sizeof(inode_t), &inode);
		
		/* Check the inode map's count with the inode.i_links_count */
		if (fsck_info->inode_map[i] != inode.i_links_count)
		{
			fprintf(stderr, "Find error: inode %d link count->", i);
			fprintf(stderr, "Should be: %d but has: %d\n",
				    fsck_info->inode_map[i], inode.i_links_count);
			inode.i_links_count = fsck_info->inode_map[i];

			/* Write correct value back to the inode */
			write_bytes((int64_t)inode_addr, sizeof(inode), &inode);
		}
	}
	return;
}