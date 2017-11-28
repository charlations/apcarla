#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
   char *haystack = argv[1];
   char *needle = argv[2];
   char *ret;

   ret = strstr(haystack, needle);
   if(ret == NULL){
       printf("Haystack: %s\nNeedle: %s\nThey don't coincide.\n", haystack, needle);
   } else{
       printf("Haystack: %s\nNeedle: %s\nThe substring is: %s\n", haystack, needle, ret);
   }
   
   return(0);
}