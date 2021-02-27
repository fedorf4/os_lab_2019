#include "revert_string.h"

void RevertString(char *str)
{
	 int i,j,l;
     char t;
     l=strlen(str);
     i=0;
     j=l-1;
     while (i<j)
     {
        t=str[i];
        str[i]=str[j];
        str[j]=t;
        i++;j--;
      }
}

