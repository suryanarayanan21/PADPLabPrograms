#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#define FILE_NAME "words.txt"
#define COUNT 100
char search_words[COUNT][20]={
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount",
"he","around","graphics","from","by","be","any","which","various","mount"
};
int count[COUNT];
int is_alpha(char c)
{
	return((c>=65&&c<=90)||(c>=97&&c<=122));
}
int is_equal(char *a,char const *key)
{
	char b[20];
	strcpy(b,key);
	int i,lena = strlen(a);
	int lenb = strlen(b);
	if(lena!=lenb)
		return 0;
	for(i=0;i<lena;i++)
	{
		if(a[i]>90)
			a[i]-=32;
		if(b[i]>90)
			b[i]-=32;
	}
	return(strcmp(a,b)==0);	
}
void readword(FILE *fp,char *temp)
{
	char c=fgetc(fp);
	int i=0;
	while(c!=EOF && is_alpha(c)!=0)
	{
		temp[i++]=c;
		c=fgetc(fp);
	}
	temp[i]='\0';	
}
int determine_count(char const *file,char const *key)
{
	int wcount=0;
	FILE *fp=fopen(file,"r");
	char temp[40]; 
	while(feof(fp)==0)
	{
		readword(fp,temp);
		if(is_equal(temp,key)!=0)
			wcount++;
	}
	return wcount;
}
int main()
{
	int nt,i;
	for(i=0;i<COUNT;i++)
		count[i]=0;
	printf("\nEnter number of threads : ");
	scanf("%d",&nt);
	double t=omp_get_wtime();
	#pragma omp parallel for num_threads(nt)
	for(i=0;i<COUNT;i++)
		count[i]=determine_count(FILE_NAME,search_words[i]);
	t=omp_get_wtime()-t;
	printf("\n\ntime taken : %lf threads:%d \n",t,nt);
	// for(i=0;i<COUNT;i++)
	// 	printf("\n%s : %d",search_words[i],count[i]);
	return 0; 
}