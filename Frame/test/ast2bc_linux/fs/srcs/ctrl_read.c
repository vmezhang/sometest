/*
 * =====================================================================================
 *
 *       Filename:  ctrl_read.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/2015 08:18:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  kristychen (chenluwen), eallyclw@gmail.com
 *        Company:  where there is a will, there is a way
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mksfs.h"
#include "dev.h"
#include "file.h"
#include "blockbitmap.h"

int ctrl_read(struct file_desc *desc, char *buf, int size)
{
	if (size > desc->inode->i_size){
		size = desc->inode->i_size;
	}
	struct sfs_inode *inode = desc->inode;
	int baddr = desc->fd_pos;
	int bondary = baddr % SFS_BLOCK_SIZE;
	int max_block = (baddr + size) / SFS_BLOCK_SIZE + 1; 
	int count = 0;
	int ux = 0;
    int ix = baddr / SFS_BLOCK_SIZE;
	for (; ix < max_block;){
		if (size + bondary <=SFS_BLOCK_SIZE){
			printf("Ctrl_read %d\n", inode->i_direct[ix]);
			count += ide_read_block(Ctrl, inode->i_direct[ix], buf, size, bondary);
			break;
		}else {
			count += ide_read_block(Ctrl, inode->i_direct[ix], buf, SFS_BLOCK_SIZE - bondary, bondary);
			ix ++;
			for (ux = 0; ux < (size - count) / SFS_BLOCK_SIZE; ux ++){
				count += ide_read_block(Ctrl, inode->i_direct[ix], buf + count, SFS_BLOCK_SIZE, 0);
			    ix ++;
		    }
			if (size != count){
				count += ide_read_block(Ctrl, inode->i_direct[ix], buf + count, size - count, 0);
				ix ++ ;
		    }
		}
	}
	desc->fd_pos += count;
	if (desc->fd_pos == inode->i_size){
		return -1;
	}
	return count;
}
