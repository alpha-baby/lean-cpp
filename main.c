#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid;
  // 创建一个进程
  pid = fork();
  if (pid == -1) {
    // 创建进程失败
    printf("fork error");
    return -1;
  }
  if (pid == 0) {
    // 子进程中
    printf("child process, %d\n", pid);
  } else {
    // 父进程中
    printf("parent process\n");
  }
  printf("all OK\n");
  return 0;
}