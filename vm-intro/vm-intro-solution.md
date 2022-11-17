# vm-intro-solution

## Code

1. The first Linux tool you should check out is the very simple tool `free`. First, type `man free` and read its entire manual page; it’s short, don’t worry!

2. Now, run `free`, perhaps using some of the arguments that might be useful (e.g., `-m`, to display memory totals in megabytes). How much memory is in your system? How much is free? Do these numbers match your intuition?

   ```shell
   free -m
   ```

   以 MB 为单位，显示内存的使用情况。

3. Next, create a little program that uses a certain amount of memory, called `memory-user.c`. This program should take one command line argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <time.h>
   #include <unistd.h>
   
   int main(int argc, char *argv[]) {
     if (argc < 3) {
       fprintf(
           stderr,
           "Usage: ./memory-user <megabytes allocated> <run time (seconds)>\n");
       exit(1);
     }
     int req_mb, run_time, arrlen;
     int i = 0;
     int *arr;
     double time_spent;
     req_mb = atoi(argv[1]);
     run_time = atoi(argv[2]);
     arrlen = req_mb * 1024 * 1024 / sizeof(int);
     arr = (int *)malloc(req_mb * 1024 * 1024);
   
     for (int j = 0; j < arrlen; j++) {
       arr[j] = 0;
     }
   
     clock_t start = clock();
     while (1) {
       time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;
       if (time_spent >= run_time) {
         break;
       }
       arr[i]++;
       i = (i + 1) % arrlen;
     }
     return 0;
   }
   ```

4. Now, while running your memory-user program, also (in a different terminal window, but on the same machine) run the `free` tool. How do the memory usage totals change when your program is running? How about when you kill the memory-user program? Do the numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?

   可以使用下面的命令每秒刷新结果。使用的内存增加了，大小符合预期。

   ```shell
   watch -n 1 "free -m"
   ```

5. Let’s try one more tool, known as `pmap`. Spend some time, and read the `pmap` manual page in detail.

6. To use `pmap`, you have to know the process ID of the process you’re interested in. Thus, first run `ps auxw` to see a list of all processes; then, pick an interesting one, such as a browser. You can also use your memory-user program in this case (indeed, you can even have that program call `getpid()` and print out its PID for your convenience).

   稍微改写一下 `memory-user.c` 。

   ```diff
     ...
   +  int mypid;
     int req_mb, run_time, arrlen;
     int i = 0;
     int *arr;
     double time_spent;
     req_mb = atoi(argv[1]);
     run_time = atoi(argv[2]);
     arrlen = req_mb * 1024 * 1024 / sizeof(int);
     arr = (int *)malloc(req_mb * 1024 * 1024);
   
   +  printf("memory-user pid: %d\n", getpid());
     ...
   ```

   `./memory-user 20 100` ，也就是分配 20MB 空间，持续 100 秒，`pmap [pid]` 输出的结果如下：

   ```
   18938:   ./memory-user 20 100
   000055604c172000      4K r---- memory-user
   000055604c173000      4K r-x-- memory-user
   000055604c174000      4K r---- memory-user
   000055604c175000      4K r---- memory-user
   000055604c176000      4K rw--- memory-user
   000055604ddc7000    132K rw---   [ anon ]
   00007fe10d277000  20492K rw---   [ anon ]
   00007fe10e67a000    136K r---- libc.so.6
   00007fe10e69c000   1388K r-x-- libc.so.6
   00007fe10e7f7000    348K r---- libc.so.6
   00007fe10e84e000     16K r---- libc.so.6
   00007fe10e852000      8K rw--- libc.so.6
   00007fe10e854000     60K rw---   [ anon ]
   00007fe10e88f000      4K r---- ld-linux-x86-64.so.2
   00007fe10e890000    152K r-x-- ld-linux-x86-64.so.2
   00007fe10e8b6000     40K r---- ld-linux-x86-64.so.2
   00007fe10e8c0000      8K r---- ld-linux-x86-64.so.2
   00007fe10e8c2000      8K rw--- ld-linux-x86-64.so.2
   00007ffd23a0d000    136K rw---   [ stack ]
   00007ffd23a4e000     16K r----   [ anon ]
   00007ffd23a52000      8K r-x--   [ anon ]
   ffffffffff600000      4K --x--   [ anon ]
    total            22976K
   ```

   可以看到其中的 `00007fe10d277000  20492K rw---   [ anon ]` 就是我们分配的内存。`anon` 指的是匿名内存。

7. Now run `pmap` on some of these processes, using various flags (like `-X`) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of code/stack/heap?

   ```
   19264:   ./memory-user 20 20
            Address Perm   Offset Device    Inode  Size   Rss   Pss Pss_Dirty Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible ProtectionKey Mapping
       5636c5aa9000 r--p 00000000  08:02  2363351     4     4     4         0          4         0        0              0             0              0               0    0       0      0           0             0 memory-user
       5636c5aaa000 r-xp 00001000  08:02  2363351     4     4     4         0          4         0        0              0             0              0               0    0       0      0           0             0 memory-user
       5636c5aab000 r--p 00002000  08:02  2363351     4     4     4         0          4         0        0              0             0              0               0    0       0      0           0             0 memory-user
       5636c5aac000 r--p 00002000  08:02  2363351     4     4     4         4          4         4        0              0             0              0               0    0       0      0           0             0 memory-user
       5636c5aad000 rw-p 00003000  08:02  2363351     4     4     4         4          4         4        0              0             0              0               0    0       0      0           0             0 memory-user
       5636c5bbc000 rw-p 00000000  00:00        0   132     4     4         4          4         4        0              0             0              0               0    0       0      0           0             0 [heap]
       7f61b378d000 rw-p 00000000  00:00        0 20492 20488 20488     20488      20488     20488        0              0             0              0               0    0       0      0           1             0 
       7f61b4b90000 r--p 00000000  08:02 38800690   136   136     1         0        136         0        0              0             0              0               0    0       0      0           0             0 libc.so.6
       7f61b4bb2000 r-xp 00022000  08:02 38800690  1388   880     6         0        880         0        0              0             0              0               0    0       0      0           0             0 libc.so.6
       7f61b4d0d000 r--p 0017d000  08:02 38800690   348   124     0         0        124         0        0              0             0              0               0    0       0      0           0             0 libc.so.6
       7f61b4d64000 r--p 001d4000  08:02 38800690    16    16    16        16         16        16        0              0             0              0               0    0       0      0           0             0 libc.so.6
       7f61b4d68000 rw-p 001d8000  08:02 38800690     8     8     8         8          8         8        0              0             0              0               0    0       0      0           0             0 libc.so.6
       7f61b4d6a000 rw-p 00000000  00:00        0    60    28    28        28         28        28        0              0             0              0               0    0       0      0           0             0 
       7f61b4da5000 r--p 00000000  08:02 38800677     4     4     0         0          4         0        0              0             0              0               0    0       0      0           0             0 ld-linux-x86-64.so.2
       7f61b4da6000 r-xp 00001000  08:02 38800677   152   152     1         0        152         0        0              0             0              0               0    0       0      0           0             0 ld-linux-x86-64.so.2
       7f61b4dcc000 r--p 00027000  08:02 38800677    40    40     0         0         40         0        0              0             0              0               0    0       0      0           0             0 ld-linux-x86-64.so.2
       7f61b4dd6000 r--p 00031000  08:02 38800677     8     8     8         8          8         8        0              0             0              0               0    0       0      0           0             0 ld-linux-x86-64.so.2
       7f61b4dd8000 rw-p 00033000  08:02 38800677     8     8     8         8          8         8        0              0             0              0               0    0       0      0           0             0 ld-linux-x86-64.so.2
       7ffce5c78000 rw-p 00000000  00:00        0   136    16    16        16         16        16        0              0             0              0               0    0       0      0           0             0 [stack]
       7ffce5dda000 r--p 00000000  00:00        0    16     0     0         0          0         0        0              0             0              0               0    0       0      0           0             0 [vvar]
       7ffce5dde000 r-xp 00000000  00:00        0     8     4     0         0          4         0        0              0             0              0               0    0       0      0           0             0 [vdso]
   ffffffffff600000 --xp 00000000  00:00        0     4     0     0         0          0         0        0              0             0              0               0    0       0      0           0             0 [vsyscall]
                                                  ===== ===== ===== ========= ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== =========== ============= 
                                                  22976 21936 20604     20584      21936     20584        0              0             0              0               0    0       0      0           1             0 KB 
   ```

   只知道有一个是栈，还有一些是 linux 链接器和 libc 的映射。其他 `vvar, vdso, vsyscall` 似乎是加速系统调用使用的映射。别的就不知道了。

8. Finally, let’s run `pmap` on your memory-user program, with different amounts of used memory. What do you see here? Does the output from `pmap` match your expectations?

   略。