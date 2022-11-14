# Homework

## Simulation

1. Run `./fork.py -s 10` and see which actions are taken. Can you predict what the process tree looks like at each step? Use the `-c` flag to check your answers. Try some different random seeds (`-s`) or add more actions (`-a`) to get the hang of it.

   ```
   $ ./fork.py -s 10 -a 6 -c
   
   ARG seed 10
   ARG fork_percentage 0.7
   ARG actions 6
   ARG action_list 
   ARG show_tree False
   ARG just_final False
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve True
   
                              Process Tree:
                                  a
   
   Action: a forks b
                                  a
                                  └── b
   Action: a forks c
                                  a
                                  ├── b
                                  └── c
   Action: c EXITS
                                  a
                                  └── b
   Action: a forks d
                                  a
                                  ├── b
                                  └── d
   Action: a forks e
                                  a
                                  ├── b
                                  ├── d
                                  └── e
   Action: e forks f
                                  a
                                  ├── b
                                  ├── d
                                  └── e
                                      └── f
   ```

2. One control the simulator gives you is the `fork_percentage`, controlled by the `-f` flag. The higher it is, the more likely the next action is a fork; the lower it is, the more likely the action is an exit. Run the simulator with a large number of actions (e.g., `-a 100`) and vary the `fork_percentage` from 0.1 to 0.9. What do you think the resulting final process trees will look like as the percentage changes? Check your answer with -c.

   略。

3. Now, switch the output by using the `-t` flag (e.g., run `./fork.py -t`). Given a set of process trees, can you tell which actions were taken?

   ```
   $ ./fork.py -t
   
   ARG seed -1
   ARG fork_percentage 0.7
   ARG actions 5
   ARG action_list 
   ARG show_tree True
   ARG just_final False
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False
   
                              Process Tree:
                                  a
   
   Action?
                                  a
                                  └── b
   Action?
                                  a
   Action?
                                  a
                                  └── c
   Action?
                                  a
                                  └── c
                                      └── d
   Action?
                                  a
                                  └── c
                                      └── d
                                          └── e
   ```

   由于这里面有随机性，每次运行结果是不一样的。

   Action:

   1. a forks b
   2. b EXITS
   3. a forks c
   4. c forks d
   5. d forks e

4. One interesting thing to note is what happens when a child exits; what happens to its children in the process tree? To study this, let's create a specific example: `./fork.py -A a+b,b+c,c+d,c+e,c-`. This example has process 'a' create 'b', which in turn creates 'c', which then creates 'd' and 'e'. However, then, 'c' exits. What do you think the process tree should like after the exit? What if you use the `-R` flag? Learn more about what happens to orphaned processes on your own to add more context.

   - 运行到最后进程树的样子：

     ```
     a
     ├── b
     ├── d
     └── e
     ```

   - 加入 `-R` 参数后，`local_reparent` 参数被设置为 `True` ，因此结果为

     ```
     a
     └── b
         ├── d
         └── e
     ```

   - 关于 [orphan process](https://en.wikipedia.org/wiki/Orphan_process) 的更多信息

     > In a Unix-like operating system any orphaned process will be immediately adopted by the special init system process: the kernel sets the parent to init. This operation is called re-parenting and occurs automatically. Even though technically the process has the "init" process as its parent, it is still called an orphan process since the process that originally created it no longer exists. In other systems orphaned processes are immediately terminated by the kernel. In modern Linux systems, an orphan process may be reparented to a "subreaper" process instead of init.

5. One last flag to explore is the `-F` flag, which skips intermediate steps and only asks to fill in the final process tree. Run `./fork.py -F` and see if you can write down the final tree by looking at the series of actions generated. Use different random seeds to try this a few times.

   ```
   $ ./fork.py -s 25 -f 0.5 -a 7 -F
   
   ARG seed 25
   ARG fork_percentage 0.5
   ARG actions 7
   ARG action_list 
   ARG show_tree False
   ARG just_final True
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False
   
                              Process Tree:
                                  a
   
   Action: a forks b
   Action: b EXITS
   Action: a forks c
   Action: a forks d
   Action: c forks e
   Action: d forks f
   Action: d EXITS
   
                           Final Process Tree?
   ```

   进程树为：

   ```
   a
   ├── c
   │   └── e
   └── f
   ```

6. Finally, use both `-t` and `-F` together. This shows the final process tree, but then asks you to fill in the actions that took place. By looking at the tree, can you determine the exact actions that took place? In which cases can you tell? In which can't you tell? Try some different random seeds to delve into this question.

   在进程退出，以及进程树中同级的进程进行 `fork` 操作的时候，顺序很难确定。例如：

   ```
   $ ./fork.py -s 14 -tF
   
   ARG seed 14
   ARG fork_percentage 0.7
   ARG actions 5
   ARG action_list 
   ARG show_tree True
   ARG just_final True
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False
   
                              Process Tree:
                                  a
   
   Action?
   Action?
   Action?
   Action?
   Action?
   
                           Final Process Tree:
                                  a
                                  ├── b
                                  └── d
                                      └── e
   ```

   可能的两个情况：

   这里 `c EXITS` 的时间不能确定。

   ```
   a forks b
   a forks c
   a forks d
   d forks e
   c EXITS
   ```

   第二种就是出现 orphan process 的情况：

   ```
   a forks c
   c forks b
   c EXITS
   a forks d
   d forks e
   ```

## Code

1. Write a program that calls `fork()` . Before calling `fork()` , have the main process access a variable (e.g., `x`) and set its value to something (e.g., `100`). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of `x` ?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int x = 0;
     int child_pid;
     x = 100;
     if ((child_pid = fork()) > 0) {
       x = 25;
     } else if (child_pid == 0) {
       x = 125;
     } else {
       printf("error\n");
       exit(1);
     }
     printf("x=%d\n", x);
   
     return 0;
   }
   ```

   两个值彼此互不影响。子进程获得父进程地址空间的一份副本，包括代码和数据段、堆、共享库、用户栈，以及打开的文件描述符。父进程和子进程最大的区别在于 PID 不同。

2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()` ? What happens when they are writing to the file concurrently, i.e., at the same time?

   ```c
   #include <fcntl.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <unistd.h>
   
   int main() {
     int child_pid;
     int fd;
   
     fd = open("tmptmp.txt", O_RDWR, 0);
   
     if ((child_pid = fork()) > 0) {
       write(fd, "this is from parent, no. 1\n", 28);
       write(fd, "this is from parent, no. 2\n", 28);
       write(fd, "this is from parent, no. 3\n", 28);
     } else if (child_pid == 0) {
       write(fd, "this is from child, no. 1\n", 27);
       write(fd, "this is from child, no. 2\n", 27);
       write(fd, "this is from child, no. 3\n", 27);
       close(fd);
     } else {
       printf("error\n");
       exit(1);
     }
     close(fd);
     return 0;
   }
   ```

   child 和 parent 都能访问 `open()` 返回的文件描述符。同时写入时，写入顺序是不固定的。

3. Write another program using `fork()` . The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

   我想出来的办法是 child 向 parent 发 SIGSTOP 或者 SIGTSTP ，然后 child 打印后再发 SIGCONT 。

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/types.h>
   #include <sys/wait.h>
   #include <unistd.h>
   
   int main() {
     int child_pid;
     int parent_pid = getpid();
   
     if ((child_pid = fork()) > 0) {
       kill(parent_pid, SIGSTOP);
       printf("goodbyte\n");
     } else if (child_pid == 0) {
       printf("hello\n");
       kill(parent_pid, SIGCONT);
     } else {
       printf("error\n");
       exit(1);
     }
     return 0;
   }
   ```

   但是这样还是会带一点额外输出。

   ```
   [1]  + 19679 suspended (signal)  ./tmpc
   hello
   goodbyte
   [1]  + 19679 done       ./tmpc
   ```

   另一种方式是使用 pipe 。参考 https://github.com/asnr/ostep/blob/master/virtualisation/5_process_api/3_fork_and_coordinate_print.c 。

4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls` . See if you can try all of the variants of `exec()` , including (on Linux) `execl()` , `execle() `, `execlp()` , `execv()` , `execvp()` , and `execvpe()` . Why do you think there are so many variants of the same basic call?

   已经整理过，略。

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/wait.h>
   #include <unistd.h>
   
   int main() {
     int child_pid;
   
     if ((child_pid = fork()) > 0) {
       int ret_pid = wait(NULL);
       printf("pid return from wait(): %d\n", ret_pid);
       printf("goodbyte\n");
     } else if (child_pid == 0) {
       int my_pid = getpid();
       printf("child pid: %d\n", my_pid);
       printf("hello\n");
     } else {
       printf("error\n");
       exit(1);
     }
     return 0;
   }
   ```

   `wait()` 返回子进程的 PID 。

   如果在 child 里面使用 `wait()` ，由于没有子进程，会返回 `-1` ，错误码为 `ECHILD` 。

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

   ```diff
   - int ret_pid = wait(NULL);
   + int ret_pid = waitpid(child_pid, NULL, 0);
   ```

   `wait(&status)` 相当于 `waitpid(-1， &status, 0)`  。 `waitpid()` 有更多的选项可以设置，以及能指定等待结束的子进程的 pid ，而不是等待所有子进程中的任意一个结束。

7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT_FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/wait.h>
   #include <unistd.h>
   
   int main() {
     int child_pid;
   
     if ((child_pid = fork()) > 0) {
       wait(NULL);
       printf("child proc is exited.\n");
     } else if (child_pid == 0) {
       close(STDOUT_FILENO);
       printf("after close STDOUT_FILENO, what happened?\n");
     } else {
       printf("error\n");
       exit(1);
     }
     return 0;
   }
   ```

   `close()` 后的 `printf` 不会输出任何东西。

   别人写的更详细的答案：

   > If the `printf` call is buffered, then `printf()` returns with no error, but the message is not printed to the terminal. If the `printf` call is *not* buffered, then `printf()` returns -1 (signaling that an error has occurred) and errno is set to 9, "bad file descriptor".

8. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the `pipe()` system call.

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int child_pid1;
     int pipe_fd[2];
     pipe(pipe_fd);
   
     if ((child_pid1 = fork()) > 0) {
       int child_pid2;
       if ((child_pid2 = fork()) > 0) {
         close(pipe_fd[0]);
         close(pipe_fd[1]);
       } else if (child_pid2 == 0) {
         // child 2 only read
         printf("hello from read proc!\n");
         close(pipe_fd[1]);
         dup2(pipe_fd[0], STDIN_FILENO);
         char msg[100];
         scanf("%[^\n]", msg);
         printf("%s\n", msg);
         close(pipe_fd[0]);
       }
     } else if (child_pid1 == 0) {
       // child 1 only write
       close(pipe_fd[0]);
       dup2(pipe_fd[1], STDOUT_FILENO);
       printf("write to other process\n");
       close(pipe_fd[1]);
     } else {
       printf("error\n");
       exit(1);
     }
     return 0;
   }
   ```

   很多程序只是演示一下效果，并没有太注重安全性。注意 `pipe_fd[0]` 是用来读，`pipe_fd[1]` 是用来写的。