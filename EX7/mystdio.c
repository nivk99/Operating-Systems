
/**
 * @file mystdio.c
 * AUTHORS: Niv Kotek - 208236315
 * 
 */
#include "mystdio.h"


myFILE* myfopen(const char *pathname, const char *mode) 
{
  if(!pathname||!mode)
  {
    return NULL;
  }

  if(!strstr(mode, "r")&&!strstr(mode, "r+")&&!strstr(mode, "w")&&!strstr(mode, "a"))
  {
    return NULL;
  }

  int fd = myopen(pathname, 1);

  if(fd==-1)
  {
    return NULL;
  }

    PmyFILE file = (PmyFILE)malloc(sizeof(myFILE));
    if(!file)
    {
        fputs("myfopen (file): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
    }
    file->size=IN[fd].size;
    file->fd=fd;
    file->fseek=0;
    file->isOpen=true;
    file->name=(char*)malloc(sizeof(char)*strlen(pathname)+1);
    file->data=(char*)malloc(sizeof(char)*IN[fd].size+1);
    if(!file->name||!file->data)
    {
		fputs("myfopen (name/data): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
    }
    file->name[strlen(pathname)]='\0';
    file->data[IN[fd].size]='\0';
    strcpy(file->name,pathname);
     strcpy(file->mode,mode);
    if (!strcmp(mode, "w"))
     {
        if(IN[fd].size!=0)
        {
             delete_str(fd);
        }
     }
     if (!strcmp(mode, "a"))
     {
        file->fseek=IN[fd].size;

     }
     else
     {
         file->fseek=0;
     }
     if(strcmp(mode, "w")&&IN[fd].size>0)
     {
        myread(fd,file->data,IN[fd].size);

     }
    return file;

}


int myfclose(myFILE *stream)
{
    if(stream->isOpen)
    {
        stream->isOpen=false;
        int fd=stream->fd;
        if(stream->size!=IN[fd].size&&strcmp(stream->mode,"r"))
        {
            mywrite(stream->fd,stream->data,stream->size);
        }
        free(stream->data);
        free(stream->name);
        free(stream);
        myclose(fd);
        return fd;
    }
    return NULL;
}



size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream)
{

    if (!strcmp(stream->mode,"w")||!strcmp(stream->mode,"a")||!ptr||!stream||nmemb==0||size==0)
    {
        return NULL;
    }
    myMemCpy_fread(ptr,nmemb*size,stream);
    return stream->fseek;


}

void myMemCpy_fread(void *dest, size_t n, myFILE *stream)
{
   char *cdest = (char *)dest;
  
   // Copy contents of src[] to dest[]
   for (size_t i = 0; i < n; i++)
   {
        if(stream->fseek>stream->size)
        {
            return;
        }
        cdest[i] = stream->data[stream->fseek];
        stream->fseek++;

   }
   
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    if (!strcmp(stream->mode,"r")||!ptr||!stream||nmemb==0||size==0)
    {
        return NULL;
    }
    char *str=(char*)calloc(nmemb+1,size);

    str[nmemb]='\0';
    myMemCpy_write(str,ptr,nmemb*size);
    if(!(str))
    {
        fputs("myfwrite (str): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
    }
    stream->size+=strlen(str);
    stream->data=(char*)realloc(stream->data,stream->size);
    if(!(stream->data))
    {
        fputs("myfwrite (str): cannot allocate memory\n",stderr); 
		exit(EXIT_FAILURE);
    }
    str[stream->size]='\0';
    strcat(stream->data,str);
    stream->fseek+=nmemb;
    free(str);
    return stream->fseek;
}

void myMemCpy_write(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}



int myfseek(myFILE *stream, long offset, int whence)
{
    int myfd=stream->fd;
    if(OF[myfd].fd != myfd||((whence!=SEEK_CUR)&&(whence!=SEEK_SET)&&(whence!=SEEK_END)))
    {
 		fputs("mylseek:: Invalid input\n",stderr); 
		exit(EXIT_FAILURE);
    }
	if(whence==SEEK_CUR)
	{
        stream->fseek+=offset;
	}
    if(whence == SEEK_SET) 
    {
		if(offset<0)
		{
 			fputs("mylseek:: Invalid input, offset<0 \n",stderr); 
			exit(EXIT_FAILURE);
		}
        stream->fseek=offset;
    }
    if (whence==SEEK_END) 
    {
        stream->fseek=offset+stream->size;
    }  
    return stream->fseek;
}


int myfscanf(myFILE *stream, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int* Pint;
    float* Pfloat ;
    char *Pchar;
    int size=strlen(format);
    int size_read=1;

    for (int i = 0; i < size; i++)
    { 
        if (format[i] == '%')
        {
            i++;
            while(format[i]==" ")
            {
                i++;
            }
            if(format[i] == 'd'){
                Pint = va_arg(arguments, int*);
                myfread(Pint, sizeof(int), size_read, stream);
            }
            if(format[i] == 'f'){
                Pfloat = va_arg(arguments, float*);
                myfread(Pfloat, sizeof(float), size_read, stream);
            }
            if (format[i] == 'c'){
                Pchar = va_arg(arguments, char*);
                myfread(Pchar, sizeof(char), size_read, stream);
            }
        }
    }
    return true;


}

int myfprintf(myFILE *stream, const char *format, ...)
{

    va_list arguments;
    va_start(arguments, format);
    int* Pint;
    float* Pfloat ;
    char *Pchar;
    int size=strlen(format);
    int size_read=1;
    char* trash;

    for (int i = 0; i < size; i++)
    { 
        if (format[i] == '%')
        {
            while(format[i]==" ")
            {
                i++;
            }
            i++;
            if(format[i] == 'd'){
                Pint = va_arg(arguments, int*);
                myfwrite(Pint, sizeof(int), size_read, stream);
            }
            if(format[i] == 'f'){
                Pfloat = va_arg(arguments, float*);
                myfwrite(Pfloat, sizeof(float), size_read, stream);
            }
            if (format[i] == 'c'){
                Pchar = va_arg(arguments, char*);
                myfwrite(Pchar, sizeof(char), size_read, stream);
            }
        }

    }
    return true;

}
