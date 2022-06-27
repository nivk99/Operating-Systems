
/**
 * @file test.c
 * AUTHORS: Niv Kotek - 208236315
 * 
 */


#include "myfs.h"
#include "mystdio.h"
void test1();
void test2();
void test3();
int main(int argc, char const *argv[])
{
	printf("start Tests\n");
	test1();
	test2();
	test3();
	printf("end Tests\n");
	return 0;
}



void test1()
{
	printf("******************start test 1**********************************\n");
	mymkfs(10000);
    mymount(NULL,"fs_data",NULL,0,NULL); 
	print_fs();
	myFILE *stream1 = myfopen("root/test1","r+");
	myFILE *stream2 = myfopen("root/test2","r+");
	printf("fd1- %d. fd2- %d.\n",stream1->fd,stream2->fd);
	char str1[]="This is a first test1 for files";
	char str2[]="This is a first test2 for files";
   	myfwrite(str1,sizeof(char),strlen(str1),stream1);
	myfwrite(str2,sizeof(char),strlen(str2),stream2);
	printf("size1- %d. size2- %d.\n",stream1->fseek,stream2->fseek);
	myfseek(stream1,0,SEEK_SET);
	myfseek(stream2,0,SEEK_SET);
	char ans1[strlen(str1)+1];
	char ans2[strlen(str2)+1];
	ans1[strlen(str1)]='\0';
	ans2[strlen(str2)]='\0';
	myfread(ans1,sizeof(char),strlen(str1),stream1);
	myfread(ans2,sizeof(char),strlen(str2),stream2);
	printf("test1: %s\n",ans1);
	printf("test2: %s\n",ans2);
	print_fs();
	myfclose(stream1);
	myfclose(stream2);
	printf("****************** end test 1**********************************\n");

}

void test2()
{
	printf("******************start test 2**********************************\n");
	myFILE *stream1 = myfopen("root/test1","r+");
	char c1=' ';
  	char c2='1';
  	char c3='2';
  	char c4='3';
	myfprintf(stream1,"%c%c%c%c",&c1,&c2,&c3,&c4);
  	c1='.';
  	c2='c';
  	c3='o';
  	c4='m';
  	myfprintf(stream1,"%c%c%c%c",&c1,&c2,&c3,&c4);
	myfseek(stream1,0,SEEK_SET);
	char str1[40+1];
	str1[40]='\0';
	myfread(str1,sizeof(char),40,stream1);
	printf("%s\n",str1);// print -> This is a first test1 for files 123.com
  	c1=' ';
  	c2=' ';
  	c3=' ';
  	c4=' ';
	myfseek(stream1,0,SEEK_SET);
	myfscanf(stream1,"%c%c%c%c",&c1,&c2,&c3,&c4);
	printf("%c%c%c%c\n",c1,c2,c3,c4);// print -> this
	printf("****************** end test 2**********************************\n");
 }

 void test3()
 {
	printf("******************start test 3**********************************\n");
	PmyDIR dir=myopendir("root");
	Pmydirent pmdir=myreaddir(dir);
	printf("name: %s\n",pmdir->d_name);
	printf("size: %d\n",pmdir->size);
	printf("fds: %d\n",pmdir->fds[0]);//1 ->There are 2 files in the folder
	printf("fds: %d\n",pmdir->fds[1]);//2 ->There are 2 files in the folder
	printf("open new dir");
	PmyDIR new_dir=myopendir("new_dir");
	Pmydirent new_pmdir=myreaddir(new_dir);
	printf("name: %s\n",new_pmdir->d_name);
	printf("size: %d\n",new_pmdir->size);
	printf("fds: %d\n",new_pmdir->fds[0]);//-1  -> Empty folder
	print_fs();

	printf("******************end test 3**************************************\n");

 }
