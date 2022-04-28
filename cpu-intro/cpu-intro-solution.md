# Homework

## Simulation

1. Run `process-run.py` with the following flags: `-l 5:100,5:100`. What should the CPU utilization be (e.g., the percent of time the CPU is in use?) Why do you know this? Use the `-c` and `-p` flags to see if you were right.

   CPU 利用率： 100% 。因为没有 IO 切换。

   ```
   $ python process-run.py -l 5:100,5:100 -pc
   
   Time        PID: 0        PID: 1           CPU           IOs
     1        RUN:cpu         READY             1          
     2        RUN:cpu         READY             1          
     3        RUN:cpu         READY             1          
     4        RUN:cpu         READY             1          
     5        RUN:cpu         READY             1          
     6           DONE       RUN:cpu             1          
     7           DONE       RUN:cpu             1          
     8           DONE       RUN:cpu             1          
     9           DONE       RUN:cpu             1          
    10           DONE       RUN:cpu             1          
   
   Stats: Total Time 10
   Stats: CPU Busy 10 (100.00%)
   Stats: IO Busy  0 (0.00%)
   ```

2. Now run with these flags: `./process-run.py -l 4:100,1:0`. These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done. How long does it take to complete both processes? Use `-c` and `-p` to find out if you were right.

   运行总时间：11 。因为默认的 IO 长度为 5 。

   ```
   $ ./process-run.py -l 4:100,1:0 -pc
   
   Time        PID: 0        PID: 1           CPU           IOs
     1        RUN:cpu         READY             1          
     2        RUN:cpu         READY             1          
     3        RUN:cpu         READY             1          
     4        RUN:cpu         READY             1          
     5           DONE        RUN:io             1          
     6           DONE       WAITING                           1
     7           DONE       WAITING                           1
     8           DONE       WAITING                           1
     9           DONE       WAITING                           1
    10           DONE       WAITING                           1
    11*          DONE   RUN:io_done             1          
   
   Stats: Total Time 11
   Stats: CPU Busy 6 (54.55%)
   Stats: IO Busy  5 (45.45%)
   ```

3. Switch the order of the processes: `-l 1:0,4:100`. What happens now? Does switching the order matter? Why? (As always, use `-c` and `-p` to see if you were right)

   运行总时间：7 。因为执行 IO 的同时， CPU 可以处理 4 条指令。

   ```
   $ ./process-run.py -l 1:0,4:100 -pc
   
   Time        PID: 0        PID: 1           CPU           IOs
     1         RUN:io         READY             1          
     2        WAITING       RUN:cpu             1             1
     3        WAITING       RUN:cpu             1             1
     4        WAITING       RUN:cpu             1             1
     5        WAITING       RUN:cpu             1             1
     6        WAITING          DONE                           1
     7*   RUN:io_done          DONE             1          
   
   Stats: Total Time 7
   Stats: CPU Busy 6 (85.71%)
   Stats: IO Busy  5 (71.43%)
   ```

4. We'll now explore some of the other flags. One important flag is `-S`, which determines how the system reacts when a process issues an I/O. With the flag set to `SWITCH_ON_END`, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (`-l 1:0,4:100 -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?

   运行总时间：11 。设置该标志位使得执行 IO 时， CPU 不会切换到另外一个进程。

   ```
   $ ./process-run.py -l 1:0,4:100 -S SWITCH_ON_END -pc
   
   Time        PID: 0        PID: 1           CPU           IOs
     1         RUN:io         READY             1          
     2        WAITING         READY                           1
     3        WAITING         READY                           1
     4        WAITING         READY                           1
     5        WAITING         READY                           1
     6        WAITING         READY                           1
     7*   RUN:io_done         READY             1          
     8           DONE       RUN:cpu             1          
     9           DONE       RUN:cpu             1          
    10           DONE       RUN:cpu             1          
    11           DONE       RUN:cpu             1          
   
   Stats: Total Time 11
   Stats: CPU Busy 6 (54.55%)
   Stats: IO Busy  5 (45.45%)
   ```

5. Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (`-l 1:0,4:100 -c -S SWITCH_ON_IO`). What happens now? Use `-c` and `-p` to confirm that you are right.

   运行总时间：7 。

   ```
   $ ./process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO -pc
   
   Time        PID: 0        PID: 1           CPU           IOs
     1         RUN:io         READY             1          
     2        WAITING       RUN:cpu             1             1
     3        WAITING       RUN:cpu             1             1
     4        WAITING       RUN:cpu             1             1
     5        WAITING       RUN:cpu             1             1
     6        WAITING          DONE                           1
     7*   RUN:io_done          DONE             1          
   
   Stats: Total Time 7
   Stats: CPU Busy 6 (85.71%)
   Stats: IO Busy  5 (71.43%)
   ```

6. One other important behavior is what to do when an I/O completes. With `-I IO_RUN_LATER`, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when you run this combination of processes? (Run `./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p`) Are system resources being effectively utilized?

   可以看到，执行 IO 的 PID 为 0 的进程，在 IO 执行完成后开始等待，直到时间 17 才执行 `RUN:io_done` 。而后的两次 IO 执行， CPU 都处于空闲状态，因此系统资源没有被充分利用。

   ```
   $ ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p
   
   Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
     1         RUN:io         READY         READY         READY             1          
     2        WAITING       RUN:cpu         READY         READY             1             1
     3        WAITING       RUN:cpu         READY         READY             1             1
     4        WAITING       RUN:cpu         READY         READY             1             1
     5        WAITING       RUN:cpu         READY         READY             1             1
     6        WAITING       RUN:cpu         READY         READY             1             1
     7*         READY          DONE       RUN:cpu         READY             1          
     8          READY          DONE       RUN:cpu         READY             1          
     9          READY          DONE       RUN:cpu         READY             1          
    10          READY          DONE       RUN:cpu         READY             1          
    11          READY          DONE       RUN:cpu         READY             1          
    12          READY          DONE          DONE       RUN:cpu             1          
    13          READY          DONE          DONE       RUN:cpu             1          
    14          READY          DONE          DONE       RUN:cpu             1          
    15          READY          DONE          DONE       RUN:cpu             1          
    16          READY          DONE          DONE       RUN:cpu             1          
    17    RUN:io_done          DONE          DONE          DONE             1          
    18         RUN:io          DONE          DONE          DONE             1          
    19        WAITING          DONE          DONE          DONE                           1
    20        WAITING          DONE          DONE          DONE                           1
    21        WAITING          DONE          DONE          DONE                           1
    22        WAITING          DONE          DONE          DONE                           1
    23        WAITING          DONE          DONE          DONE                           1
    24*   RUN:io_done          DONE          DONE          DONE             1          
    25         RUN:io          DONE          DONE          DONE             1          
    26        WAITING          DONE          DONE          DONE                           1
    27        WAITING          DONE          DONE          DONE                           1
    28        WAITING          DONE          DONE          DONE                           1
    29        WAITING          DONE          DONE          DONE                           1
    30        WAITING          DONE          DONE          DONE                           1
    31*   RUN:io_done          DONE          DONE          DONE             1          
   
   Stats: Total Time 31
   Stats: CPU Busy 21 (67.74%)
   Stats: IO Busy  15 (48.39%)
   ```

7. Now run the same processes, but with `-I IO_RUN_IMMEDIATE` set, which immediately runs the process that issued the I/O. How does this behavior differ? Why might running a process that just completed an I/O again be a good idea?

   立即执行发起 IO 的进程提高了运行效率。因为这样该进程可以再次发起 IO 请求， CPU 利用空闲期能够处理其他指令。

   ```
   $ ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -c -p 
   
   Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
     1         RUN:io         READY         READY         READY             1          
     2        WAITING       RUN:cpu         READY         READY             1             1
     3        WAITING       RUN:cpu         READY         READY             1             1
     4        WAITING       RUN:cpu         READY         READY             1             1
     5        WAITING       RUN:cpu         READY         READY             1             1
     6        WAITING       RUN:cpu         READY         READY             1             1
     7*   RUN:io_done          DONE         READY         READY             1          
     8         RUN:io          DONE         READY         READY             1          
     9        WAITING          DONE       RUN:cpu         READY             1             1
    10        WAITING          DONE       RUN:cpu         READY             1             1
    11        WAITING          DONE       RUN:cpu         READY             1             1
    12        WAITING          DONE       RUN:cpu         READY             1             1
    13        WAITING          DONE       RUN:cpu         READY             1             1
    14*   RUN:io_done          DONE          DONE         READY             1          
    15         RUN:io          DONE          DONE         READY             1          
    16        WAITING          DONE          DONE       RUN:cpu             1             1
    17        WAITING          DONE          DONE       RUN:cpu             1             1
    18        WAITING          DONE          DONE       RUN:cpu             1             1
    19        WAITING          DONE          DONE       RUN:cpu             1             1
    20        WAITING          DONE          DONE       RUN:cpu             1             1
    21*   RUN:io_done          DONE          DONE          DONE             1          
   
   Stats: Total Time 21
   Stats: CPU Busy 21 (100.00%)
   Stats: IO Busy  15 (71.43%)
   ```

8. Now run with some randomly generated processes: `-s 1 -l 3:50,3:50` or `-s 2 -l 3:50,3:50` or `-s 3 -l 3:50,3:50`. See if you can predict how the trace will turn out. What happens when you use the flag `-I IO_RUN_IMMEDIATE` vs. `-I IO_RUN_LATER`? What happens when you use `-S SWITCH_ON_IO` vs. `-S SWITCH_ON_END` ?

   - 无法预测，因为 IO 和 CPU 指令出现的次数和顺序是不一定的，换随机数种子就变了。
   - `IO_RUN_IMMEDIATE` 的效率高于 `IO_RUN_LATER` ， `SWITCH_ON_IO` 的效率高于 `SWITCH_ON_END` 。

