// #includes omitted!
// Please read the man pages for the functions.
#include "secret_headers.h"
#include <sys/stat.h>
void getAndPrintGroup(char *dir)
{
    struct stat info; 
    stat(dir, &info);
    struct group *grp = getgrgid(info.st_gid);
    if (grp) {
        printf(" %s ", grp->gr_name);
    } else {
        printf("No group name");
    }
}

void getAndPrintUserName(char *dir)
{
    struct stat info;
    stat(dir, &info);
    struct passwd *pw = getpwuid(info.st_uid);
    if (pw) {
        printf(" %s ", pw->pw_name);
    } else {
        perror("Hmm not found???");
    }
}

/*int main()
{
    getAndPrintGroup(1001);
    getAndPrintGroup(514378);
    getAndPrintGroup(103);
    getAndPrintGroup(1000);

    getAndPrintUserName(59894);
    getAndPrintUserName(23524);
    getAndPrintUserName(20746);
    getAndPrintUserName(5970);
    getAndPrintUserName(10485);

    return 0;
}*/
