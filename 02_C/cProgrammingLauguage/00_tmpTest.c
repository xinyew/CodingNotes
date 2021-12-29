#include <stdio.h>
int main() {
   int day, year;

   char monthname[20];
   scanf("%d %s %d", &day, monthname, year);
   printf("%d %s %d\n", day, monthname, year);
}