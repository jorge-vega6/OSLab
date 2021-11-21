// fs.cpp: File System

#include "sfs/fs.h"

// #include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int * bitmap = NULL; //free block bitmap
int bitmapLength; //size of bitmap

Disk * currentDisk; //keep access to the current disk

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
    //can't mount disk if it's already mounted
    if (disk->mounted(disk)){
        return false;
    }

    // Read superblock
    Block block;
    disk->readDisk(disk, 0, block.Data);

    //check magic number
    if (block.Super.MagicNumber != MAGIC_NUMBER){
        return false;
    }

    // Set device and mount
    currentDisk = disk;
    disk->mount(disk);

    // Allocate free block bitmap
    bitmap = calloc(block.Super.Blocks, sizeof(int));
    bitmapLength = block.Super.Blocks;

    //scan blocks to fill in bitmap
    Block currentBlock;
    Inode currentInode;

    for (int i = 1; i < block.Super.InodeBlocks; i++){
        disk->readDisk(disk, i, currentBlock.Data);
        for (int j = 0; j < INODES_PER_BLOCK; j++){
            currentInode = currentBlock.Inodes[j];
            if (currentInode.Valid){
                bitmap[i] = 1; //set entry in bitmap
            }
        }
    }

    return disk->mounted(disk); //verify that it has actually been mounted
}

// Create inode ----------------------------------------------------------------

size_t create() {
    // Locate free inode in inode table
    if (bitmap == NULL){ //no bitmap is set
        return -1;
    } else if (!(currentDisk->mounted(currentDisk))){ //disk hasn't been mounted
        return -1;
    }

    Block block;
    currentDisk->readDisk(currentDisk, 0, block.Data);

    //traverse blocks and find empty inode
    for (int i = 1; i < block.Super.Blocks; i++){
        currentDisk->readDisk(currentDisk, i, block.Data); //read current inode block
        Inode currentInode;
        
        // Record inode if found
        for (int j = 0; j < POINTERS_PER_BLOCK; j++){
            if (j == 0 && i == 1){
                j = 1;
            }

            currentInode = block.Inodes[j]; //read the current inode
            if (currentInode.Valid == 0){ //invalid, so we found an empty inode to use
                currentInode.Valid = 1; //set valid bit
                currentInode.Size = 0; //init size to 0
                memset(currentInode.Direct, 0, sizeof(currentInode.Direct));
                currentInode.Indirect = 0;
                bitmap[i] = 1; //set entry in bitmap
                block.Inodes[j] = currentInode; //set inode in block
                currentDisk->writeDisk(currentDisk, i, block.Data); //write the data

                return j + (i - 1) * INODES_PER_BLOCK; //return inumber
            }
        }
    }

    return -1; //return -1 as a default if all fails
}

// Remove inode ----------------------------------------------------------------

bool removeInode(size_t inumber) {
    // Load inode information
    int iNodeBlock = (inumber + INODES_PER_BLOCK - 1) / INODES_PER_BLOCK;
    Block block; 
    currentDisk->readDisk(currentDisk, 0, block.Data);

    //check if calculated inode block number exceeds
    if (iNodeBlock > block.Super.InodeBlocks){
        return false;
    }

    currentDisk->readDisk(currentDisk, iNodeBlock, block.Data);
    Inode currentInode = block.Inodes[inumber % INODES_PER_BLOCK];

    if (currentInode.Valid){
        // Free direct blocks
        memset(currentInode.Direct, 0, POINTERS_PER_INODE);

        // Free indirect blocks
        block.Pointers[currentInode.Indirect] = NULL;

        // Clear inode in inode table
        currentInode = (Inode){0};
        block.Inodes[inumber % INODES_PER_BLOCK] = currentInode;
        currentDisk->writeDisk(currentDisk, iNodeBlock, block.Data);
        return true;
    } else {
        return false;
    }
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
