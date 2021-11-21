// fs.cpp: File System

#include "sfs/fs.h"

// #include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int * bitmap = NULL; //free block bitmap

// Debug file system -----------------------------------------------------------

void debug(Disk *disk) {
    Block block;

    // Read Superblock
    
    disk->readDisk(disk, 0, block.Data);
    
    printf("SuperBlock:\n");
    printf("    magic number is %s\n", (block.Super.MagicNumber == MAGIC_NUMBER ? "valid" : "invalid"));
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes);
    
    
    // Read Inode blocks
    int inodeBlocksNum = block.Super.InodeBlocks; //acquire number of inodes in inode block
    for (int i = 1; i <= inodeBlocksNum; i++){ //traverse inodes in disk
        disk->readDisk(disk, i, block.Data); //read next block onto block.Data (block of inodes)
        for (unsigned int j = 0; j < INODES_PER_BLOCK; j++){ //traverse the inodes in the block of inodes
            Inode inode = block.Inodes[j]; //get current inode
            if (inode.Valid == 0){
                continue; //skip inode if not valid
            }
            printf("Inode %d:\n", j); //print valid inode
            printf("    size: %d bytes\n", inode.Size);
            int directBlockCount = 0;
            for (unsigned int k = 0; k < POINTERS_PER_INODE; k++){ //traverse pointers to gain count
                if (inode.Direct[k]){
                    directBlockCount++;
                }
            }
            printf("    direct blocks: %d\n", directBlockCount);
        }
    }
}

// Format file system ----------------------------------------------------------

bool format(Disk *disk) {
    
    if (disk->mounted(disk)){
        return false; //can't format disk if it's already mounted
    } else if (bitmap != NULL){
        return false; //return false if there's already a bitmap
    }

    // Write superblock
    Block block;
    block.Super.MagicNumber = MAGIC_NUMBER; //set magic number for superblock
    block.Super.Blocks = disk->size(disk); //get number of blocks in disk
    uint32_t newInodeBlocks = block.Super.Blocks / 10;
    if (block.Super.Blocks % 10 != 0){ //if it's not divisible by 10, it has a decimal part
        newInodeBlocks++; //add 1 to round up
    }
    block.Super.InodeBlocks = newInodeBlocks;
    block.Super.Inodes = INODES_PER_BLOCK * block.Super.InodeBlocks; //set total number of inodes

    disk->writeDisk(disk, 0, block.Data);

    // Clear all other blocks
    Block emptyBlock;
    memset(emptyBlock.Data, 0, BLOCK_SIZE); //fill the empty block with 0s
    
    //write to the disk the empty blocks and clear them
    for (int i = 0; i < disk->size(disk); i++){
        disk->writeDisk(disk, i, emptyBlock.Data);
    }
    
    return true;
}



// Mount file system -----------------------------------------------------------

bool mount(Disk *disk) {
    // Read superblock

    // Set device and mount

    // Copy metadata

    // Allocate free block bitmap

    return true;
}

// Create inode ----------------------------------------------------------------

size_t create() {
    // Locate free inode in inode table

    // Record inode if found
    return 0;
}

// Remove inode ----------------------------------------------------------------

bool removeInode(size_t inumber) {
    // Load inode information

    // Free direct blocks

    // Free indirect blocks

    // Clear inode in inode table
    return true;
}

// Inode stat ------------------------------------------------------------------

size_t stat(size_t inumber) {
    // Load inode information
    return 0;
}

// Read from inode -------------------------------------------------------------

size_t readInode(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode information

    // Adjust length

    // Read block and copy to data
    return 0;
}

// Write to inode --------------------------------------------------------------

size_t writeInode(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode
    
    // Write block and copy to data
    return 0;
}
