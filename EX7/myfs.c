/**
 * @file myfs.c
 * AUTHORS: Niv Kotek - 208236315
 * 
 */


#include "myfs.h"
void mymkfs(int s)
{
	SB=(Psuperblock)malloc(sizeof(superblock)*1);
	if(!SB)
	{
		fputs("create_fs (SB): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
    SB->num_inodes = (s*0.1)/sizeof(inode);//10%
    SB->num_blocks = (s-(sizeof(superblock)+SB->num_inodes))/sizeof(block);
	SB->size_blocks=sizeof(block);
	create_fs();
	create_folder("root");
}


void create_fs()
{
	OF=(Pmyopenfile)malloc(sizeof(myopenfile)*FILESIZE);
	if(!OF)
	{
		fputs("create_fs(OF): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < FILESIZE; i++)
	{
		OF[i].pos=0;
		OF[i].fd=-1;
	}

	IN=(Pinode)malloc(sizeof(inode)*SB->num_inodes);
	if(!IN)
	{
		fputs("create_fs(IN): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < SB->num_inodes; i++)
	{
		IN[i].size=0;
		IN[i].first_block=-1;
		strcpy(IN[i].name,"");
		IN[i].isDir=false;
	}
	BL=(Pblock)malloc(sizeof(block)*SB->num_blocks);

	if(!BL)
	{
		fputs("create_fs(BL): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < SB->num_blocks; i++)
	{
		strcpy(BL[i].data,"");
		BL[i].next_block=-1;
	}


}

PmyDIR create_folder(char *name)
{
	int fd = allocate_file(name);
    IN[fd].isDir = true;

	int d=myopen(name,0);
    Pmydirent dir = (Pmydirent)malloc(sizeof(mydirent));
    if(!dir)
    {
		fputs("create_folder (curr): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
    }
	strcpy(dir->d_name, name);
    dir->size = 0;
    for (int i = 0; i < DIRSIZE; i++)
    {
        dir->fds[i] = -1;        
    }
    char* temp = (char*)dir;
    for (int i = 0; i < sizeof(mydirent); i++)
    {
        write_byte(fd, i, temp[i]);
    }
    free(dir);
	PmyDIR myD = (PmyDIR)malloc(sizeof(myDIR));
	if(!myD)
	{
		fputs("create_folder (myD): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	myD->fd=fd;
	return myD;
}


void write_byte(int filenum,int pos,char data)
{
    int rb = IN[filenum].first_block;
    while (pos>=BLOCKSIZE) 
    {
        pos-=BLOCKSIZE;

        if (BL[rb].next_block == -2) 
        { 
            BL[rb].next_block = find_empty_block(); 
            rb = BL[rb].next_block;
            BL[rb].next_block = -2; 
        } 
        else 
        {
            rb = BL[rb].next_block;
        }
    }
	IN[filenum].size++;
    BL[rb].data[pos] = data;
}



int allocate_file(char name[8])
{
	int in=find_empty_inode();
	int bl=find_empty_block();
	if(in==-1||bl==-1)
	{
		fputs("There is no more room in memory\n",stderr); 
		exit(EXIT_FAILURE);
	}

	IN[in].first_block=bl;
	strcpy(IN[in].name,name);
	BL[bl].next_block=-2;

	return in;
}



int mymount(const char *source, const char *target,const char *filesystemtype, unsigned long mountflags, const void *data)
{
    if (source!=NULL)
    {
        mount_fs(source);
    }
	 if (target!=NULL)
    {
        sync_fs(target);
    }
}


void mount_fs(char *fd)
{

	FILE *file;
	file=fopen(fd,"r");
	if(!file)
	{
		fputs("fopen\n",stderr); 
		exit(EXIT_FAILURE);
	}
	IN = malloc(sizeof(inode) * SB->num_inodes);
	if(!IN)
	{
		fputs("mount_fs (IN): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}

    BL = malloc(sizeof(block) * SB->num_blocks);
	if(!BL)
	{
		fputs("mount_fs (BL): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	fread(SB,sizeof(superblock),1,file);
	fread(IN,sizeof(inode),SB->num_inodes,file);
	fread(BL,sizeof(block),SB->num_blocks,file);
	

	fclose(file);

}


void sync_fs(char *fd)
{

	FILE *file;
	file=fopen("fs_data","w+");
	if(!file)
	{
		fputs("fopen\n",stderr); 
		exit(EXIT_FAILURE);
	}
	fwrite(SB,sizeof(superblock),1,file);
	fwrite(IN,sizeof(inode),SB->num_inodes,file);
	fwrite(BL,sizeof(inode),SB->num_blocks,file);
	
	fclose(file);


}


int myopen(const char *pathname, int flags) 
{

    char *temp_pathname=(char*)calloc(strlen(pathname)+1,sizeof(char));
	if(!temp_pathname)
	{
		fputs("myopen (temp_pathname): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
    strcpy(temp_pathname, pathname);
	temp_pathname[strlen(pathname)]='\0';
	char *token = strtok(temp_pathname, "/");
	char *name=(char*)calloc(NAMESIZE+1,sizeof(char));
	char *dir_name=(char*)calloc(NAMESIZE+1,sizeof(char));
	if(!name||!dir_name)
	{
		fputs("myopen (name\temp_name): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	name[strlen(token)]='\0';
	dir_name[strlen(token)]='\0';
	strcpy(dir_name, token);
	token = strtok(NULL, "/");
	if(token!=NULL)
	{
		strcpy(name, token);
	}

	free(temp_pathname);
    for (int i = 0; i < SB->num_inodes; i++)
    {
        if (!strcmp(IN[i].name,name)) 
		{
			free(dir_name);
			free(name);
			int in=find_empty_openfile();
			if(in==-1)
			{
				fputs("There is no more space in the open files table\n",stderr); 
				exit(EXIT_FAILURE);
			}
			OF[in].fd=i;
			OF[in].pos=0;
            return i; 
        }
    }

		 int fd = allocate_file(name);
		 int dir=get_fdDir(dir_name);
		 Pmydirent pdir=_myreaddir(dir);
		 pdir->fds[pdir->size]=fd;
		 pdir->size++;
		 int in=find_empty_openfile();
		OF[in].fd=fd;
		OF[in].pos=0;
		 return fd; 


	free(name);
	return -1;
}


int myclose(int myfd) {
	if( OF[myfd].fd==-1)
	{
		return -1;
	}
    OF[myfd].fd = -1;
    OF[myfd].pos = -1;
	return myfd;
}

void _myMemCpy(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}


ssize_t myread(int myfd, void *buf, size_t count)
{

	if (OF[myfd].fd != myfd) 
    {
		fputs("The file is not open\n",stderr); 
		exit(EXIT_FAILURE);
    }
	char *temp=(char*)calloc(count+1,sizeof(char));
	if(!temp)
	{
		fputs("myread (temp): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	temp[count] = '\0';
	int this=IN[myfd].first_block;
	int pos=OF[myfd].pos;
    for (int i = 0; i < count; i++)
    {
		while (pos>=BLOCKSIZE)
		{
			pos-=BLOCKSIZE;
			this=BL[this].next_block;
		}
		temp[i]=BL[this].data[pos];
		pos++;
    }
	
	_myMemCpy(buf,temp,count);
	free(temp);
	OF[myfd].pos+=count;
    return OF[myfd].pos;
}

ssize_t mywrite(int myfd, const void *buf, size_t count)
{

	// if(strlen(buf)<count)
	// {
	// 	fputs("The size of the array is small\n",stderr); 
	// 	exit(EXIT_FAILURE);
	// }

	if (OF[myfd].fd != myfd) 
    {
		fputs("The file is not open\n",stderr); 
		exit(EXIT_FAILURE);
    }
	char *temp=(char*)calloc(count+1,sizeof(char));
	if(!temp)
	{
		fputs("myread (temp): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
	}
	temp[count] = '\0';
	strcpy(temp,buf);
	int this=IN[myfd].first_block;
	int pos=OF[myfd].pos;
    for (int i = 0; i < count; i++)
    {
		while (pos>=BLOCKSIZE)
		{
			pos-=BLOCKSIZE;
			if(BL[this].next_block==-2)
			{
				BL[this].next_block = find_empty_block(); 
            	this = BL[this].next_block;
           	 	BL[this].next_block = -2; 
			}
			else
			{
				this=BL[this].next_block;

			}

		}
		BL[this].data[pos] =temp[i];
		pos++;
	}
		free(temp);
		OF[myfd].pos+=count;
		IN[myfd].size+=count;
		return OF[myfd].pos;
}




off_t mylseek(int myfd, off_t offset, int whence)
{
    if(OF[myfd].fd != myfd||((whence!=SEEK_CUR)&&(whence!=SEEK_SET)&&(whence!=SEEK_END)))
    {
 		fputs("mylseek:: Invalid input\n",stderr); 
		exit(EXIT_FAILURE);
    }
	if(whence==SEEK_CUR)
	{
		OF[myfd].pos+=offset;
	}
    if(whence == SEEK_SET) 
    {
		if(offset<0)
		{
 			fputs("mylseek:: Invalid input, offset<0 \n",stderr); 
			exit(EXIT_FAILURE);
		}
        OF[myfd].pos = offset;
    }
    if (whence==SEEK_END) 
    {
        OF[myfd].pos = offset+IN[myfd].size;
    }  
    return OF[myfd].pos;
}


int find_empty_inode()
{
	for(int i=0;i<SB->num_inodes;i++)
	{
		if(IN[i].first_block==-1)
		{
			return i;
		}

	}
	printf("empty_inode");
	return -1;
}

int find_empty_block()
{
	for (size_t i = 0; i < SB->num_blocks; i++)
	{
		if(BL[i].next_block==-1)
		{
			return i;
		}
	}
	printf("empty_block");
	return -1;
	
}

int find_empty_openfile()
{
	for (size_t i = 0; i < FILESIZE; i++)
	{
		if(OF[i].fd==-1)
		{
			return i;
		}
	}
	printf("empty_openfile");
	return -1;
	

}

struct mydirent *_myreaddir(int fd)
{
    return (Pmydirent)BL[IN[fd].first_block].data;

}

struct mydirent *myreaddir(myDIR *dirp)
{
    return _myreaddir(dirp->fd);
}



myDIR *myopendir(const char *name)
{
	for (size_t i = 0; i < SB->num_inodes; i++)
	{
		if(strstr(IN[i].name,name)&&IN[i].isDir)
		{
			PmyDIR myD = (PmyDIR)malloc(sizeof(myDIR));
			if(!myD)
			{
				fputs("create_folder (myD): cannot allocate memory\n",stderr); 
				exit(EXIT_FAILURE);
			}
			myD->fd=i;
			return myD;
		}
	}
	return create_folder(name);
	

}





int myclosedir(myDIR *dirp)
{
	Pmydirent temp=myreaddir(dirp);
	for (size_t i = 0; i < DIRSIZE; i++)
	{
		if(temp->fds[i]!=-1)
		{
			myclose(temp->fds[i]);
		}
	}
	
	return 0;
}



int get_block_num(int file, int offset)
{
	int togo=offset;
	int bn=IN[file].first_block;
	while (togo>0)
	{
		bn=BL[bn].next_block;
	}
	return bn;

}

int get_fdDir(char *name)
{
	for (size_t i = 0; i <SB->num_inodes; i++)
	{
		if(strstr(name,IN[i].name))
		{
			return i;
		}
	}
	return -1;
	

}

void delete_str(int fd)
{
	int this=IN[fd].first_block;

	while (BL[this].next_block!=-1)
	{
		memset(BL[this].data, 0,BLOCKSIZE);
		int temp=this;
		this=BL[this].next_block;
		BL[temp].next_block=-1;
	}
	IN[fd].size=0;
	IN[fd].first_block=find_empty_block();

}



void print_fs()
{
    printf("Superblock info\n");
    printf("\tnum inodes %d\n", SB->num_inodes);
    printf("\tnum blocks %d\n", SB->num_blocks);
    printf("\tsize blocks %d\n", SB->size_blocks);

    printf("\tInodes\n");
    for (int i = 0; i < SB->num_inodes; i++)
    {
        printf("\tsize: %d block: %d name: %s\n",IN[i].size,IN[i].first_block,IN[i].name);
    }

	printf("\tBlocks\n");
    for (int i = 0; i < SB->num_blocks; i++)
    {
        printf("\tblock num: %d next block %d\n", i, BL[i].next_block);
    }
	printf("\tEND\n");
}



// int main(int argc, char const *argv[])
// {
// }
//     mymkfs(10000);
//     mymount(NULL,"fs_data",NULL,0,NULL); 
// 	int file1 = myopen("root/test1", 1);
// 	int file2 = myopen("root/test2", 1);
//    mywrite( file1,"This is a first test1 for files\n",35);
//     mywrite( file2,"This is a first test2 for files\n",35);
//    myclose( file1);
// 	myclose( file2);
//    int y1 = myopen("root/test1", 1);
//     int y2 = myopen("root/test2", 1);
//    char b1[35];
//    char b2[35];
//    myread(y1,b1,35);
//     myread(y2,b2,35);
//    printf("%s\n",b1);
//     printf("%s\n",b2);
//    	print_fs();

// 	return 0;
// }

