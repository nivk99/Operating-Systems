/**
 * @file myfs.h
 * AUTHORS: Niv Kotek - 208236315
 * 
 * 
 * https://www.youtube.com/watch?v=n2AAhiujAqs
 */


#pragma once
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#define BLOCKSIZE 512
#define NAMESIZE 8
#define DIRSIZE 5
#define FILESIZE 10000

typedef struct _superblock
{
	int num_inodes;
	int num_blocks;
	int size_blocks;
}superblock,*Psuperblock;

typedef struct _inode
{
	int size;
	int first_block;
	char name[NAMESIZE];
	bool isDir;

}inode, *Pinode;

typedef struct _block
{
	int next_block;
	char data[BLOCKSIZE];
} block,*Pblock;

typedef struct _myopenfile
{
    int fd;
    int pos;

}myopenfile,*Pmyopenfile;


typedef struct _mydirent
 { 
    int size;
    int fds[DIRSIZE];
    char d_name[DIRSIZE];

}mydirent,*Pmydirent;

typedef struct _myDIR 
{ 
    int fd;
	
}myDIR,*PmyDIR;

Psuperblock SB;
Pinode IN;
Pblock BL;
Pmyopenfile OF;

void mymkfs(int s); 

int mymount(const char *source, const char *target,
const char *filesystemtype, unsigned long
mountflags, const void *data);

int myopen(const char *pathname, int flags);

int myclose(int myfd);

ssize_t myread(int myfd, void *buf, size_t count);

ssize_t mywrite(int myfd, const void *buf, size_t count);

off_t mylseek(int myfd, off_t offset, int whence);

myDIR *myopendir(const char *name);

struct mydirent *myreaddir(myDIR *dirp);

int myclosedir(myDIR *dirp);

//Help functions

struct mydirent *_myreaddir(int fd);

void create_fs();

PmyDIR create_folder(char *name);

void mount_fs(char *fd);

void sync_fs(char *fd);

int allocate_file(char name[8]);

int find_empty_inode();

int find_empty_block();

int find_empty_openfile();

int get_fdDir(char *name);

void write_byte(int filenum,int pos,char data);

void delete_str(int fd);

void _myMemCpy(void *dest, void *src, size_t n);

