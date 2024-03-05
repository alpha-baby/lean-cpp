#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (access("/dev", F_OK) == 0) {
        chdir("/dev");
    }
    DIR *dp;

    struct dirent *dirp;
    if (argc != 2) {
        printf("usage: ls directory_name");
        return 1;
    }
    if ((dp = opendir(argv[1])) == NULL) {
        printf("can't open %s\n", argv[1]);
        perror("can't open file\n");
        printf("errno %d\n", errno);
        printf("err str %s\n", strerror(errno));
        return 2;
    }
    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp->d_name);
    }
    closedir(dp);
    return 0;
}
