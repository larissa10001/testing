/*
 * From: https://gist.github.com/marcetcheverry/991042
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define COLSIZE 9
#define ROWSIZE 96
 void append(char* s, char c);
void printHistogram(int hist[], int index);
int indexColum(int colValue);
int convertTominute(int hr, int min);
int timeDiv(char * tm);
int indexRow(int time);

int main(int argc, const char *argv[])
{
    //const char *filepath = "/tmp/mmapped.bin";
     const char *filepath = "/Users/larissaba/Documents/mmap/stat.txt";

    int fd = open(filepath, O_RDONLY,S_IRUSR|S_IWUSR);
	//struct stat sb;

    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    struct stat fileInfo = {0};

    if (fstat(fd, &fileInfo) == -1)
    {
        perror("Error getting the file size");
        exit(EXIT_FAILURE);
    }

    if (fileInfo.st_size == 0)
    {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(EXIT_FAILURE);
    }

    printf("File size is %ji\n", (intmax_t)fileInfo.st_size);

    char *map = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

	// int data[fileInfo.st_size];
	
	
     int time[5];
	int histogram[COLSIZE * ROWSIZE] = {0};
	
	 char hr[4];
	 int i,j,k;
     int word =0;
     char str[9];
     int convertTime;
   

    for (off_t i = 0; i < fileInfo.st_size; i++)
   {
       
        //printf("inside of the loop");
        
       // printf("Found character %c at %ji\n", map[i], (intmax_t)i);


        if ( isspace(map[i]) || ispunct(map[i]) )
         {
              
           if ( word )
            {
               //printf("%s",str); 
              //  str[i] ='\0';
              // printf("string val is: %d",atoi(str));
           // printf("String value casted is: %d",atoi(str));
              // histogram[k] = atoi(str);
              // printf("histo : %d",histogram[k]);
                
             time[0] = str[0] - 48;

            // printf("%d",time[0]);
			 time[1] = str[1] - 48;
			 time[2] = str[2] -48;
			 time[3] = str[3] -48;
			time[4] = '\0';
            convertTime = (time[0]*10+time[1])*60 + (time[2]*10 +time[3]);
            //printf("converted time is: %d",convertTime);

            hr[0] = str[4];
			hr[1] = str[5];
			if(str[6]!='\0'){
				hr[2] = str[6];
				hr[3] = '\0';
			}
			else{hr[2]= '\0';}
            //printf("heart rate  is: %d",atoi(hr));

            int rowIndex, colIndex;
			rowIndex = (int)(convertTime/15);
            //printf("row index%d",rowIndex);
			colIndex = indexColum(atoi(hr));
           // printf("col index returned is; %d",colIndex);
			if(colIndex == -1){break;}
			else
			{   //printf("before increment SIZE IS: %d:", histogram[rowIndex*COLSIZE+colIndex]);
                histogram[rowIndex*COLSIZE+colIndex]++;
                // printf("after increment SIZE IS: %d:", histogram[rowIndex*COLSIZE+colIndex]);
            
            }

               word = 0;
               putchar('\n');
               memset(str, 0, sizeof str);
              // printf("after clear value is: %s\n",str);
            }
         }
         else
         {
           word = 1;
           append(str,map[i]);
           //printf("%c",str[i]); 
         // putchar(map[i]);
         }

   }
   printHistogram(histogram,0);
   
	
	//printf("\n");
    // Don't forget to free the mmapped memory
    if (munmap(map, fileInfo.st_size) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);

    return 0;

}
void append(char* s, char c)
{
       // printf("inside of append");
        int length = strlen(s);
        s[length] = c;
        s[length+1] = '\0';
       // printf("value appended: %s \t", s);
}


int indexRow(int time){
	return (int)(time/15);
}
// int convertTominute(int hr, int min){

// 	return 60*hr + min;
// }
// return the index of the column
int indexColum(int colValue){
    //printf("inside of the indexcolumn");
	if(colValue>=40 && colValue<50 )
		return 0;
	else if(colValue>=50 && colValue<60 )
		return 1;
	else if(colValue>=60 && colValue<70 ){
    // printf("val returned is 2");
		return 2;
    }
	else if(colValue>=70 && colValue<80 )
		return 3;
	else if(colValue>=80 && colValue<90 )
		return 4;
	else if(colValue>=90 && colValue<100 )
		return 5;
	else if(colValue>=100 && colValue<110 )
		return 6;
	else if(colValue>=110 && colValue<120 )
		return 7;
	else if(colValue>=120 && colValue<130 )
		return 8;
	else 
		return -1;


}
// this is printing
void printHistogram(int hist[], int index){

int i,n,j;
//int start = startMin;
//int end= endMin;
// display the table header...
printf("%s%13s%17s\n","Element/index", "Value", "Histogram");
n=index;
// do the iteration, outer for loop for time increment
// for(n=startIndex; n<=endIndex; n++){
// outer loop
        for(i=0; i <= COLSIZE; i++)
    {
    printf("%9d%15d ", i, hist[n*COLSIZE+i]);
    
    // inner for loop, for every row, read column by column and print the bar...
            for(j = 1; j<= hist[n*COLSIZE+i]; j++)
            // print the 'bar', and repeat...
            printf("*");
    // go to new line for new row, and repeats...
    printf("\n");
    }

//}


}


