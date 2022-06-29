# Homework

## Simulation

1. Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os. 

    MLFQ 的模拟代码参数相对复杂，使用如下命令实现题目要求的条件。

    ```bash
    ./mlfq.py -j 2 -n 2 -s 100 -M 0
    ```

    `-j` 和 `-n` 分别代表 jobs 和 queues 的数量， `-s` 是随机数种子， `-M` 是 max I/O frequency 。

    这样设置，没有 I/O boost ，运行完一次规定的时间片数量就降优先级(allotment=1)。这个调度过程比较容易分析。

2. How would you run the scheduler to reproduce each of the examples in the chapter? 

3. How would you configure the scheduler parameters to behave just like a round-robin scheduler? 

    设置队列数量为 1 ，用 `-q` 确定时间片的长度。

    ```bash
    ./mlfq.py -j 4 -n 1 -s 100 -M 0 -q 5 -c
    ```

4. Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval. 

    先举一个简单的例子说明

    ```bash
    ./mlfq.py -l 0,10,3:0,15,0 -Q 4,8,12 -i 1 -I -S -c
    ```

    上面的命令运行两个任务，第一个任务从时间 0 开始，运行时长 10 ，每隔时长 3 发起一次 I/O 请求， I/O 请求持续时长为 1 ；第二个任务从时间 0 开始，运行时长 15 ，不发起 I/O 请求。

    队列设置：一共有 3 条队列，每条队列允许每次分别执行 4, 8, 12 长度的时间片。 `-S` 参数允许发起 I/O 的任务仍位于原本的优先级队列中。 `-I` 参数允许执行完 I/O 的任务回到执行队列的首位。

    执行结果如下：

    ```
    Here is the list of inputs:
    OPTIONS jobs 2
    OPTIONS queues 3
    OPTIONS allotments for queue  2 is   1
    OPTIONS quantum length for queue  2 is   4
    OPTIONS allotments for queue  1 is   1
    OPTIONS quantum length for queue  1 is   8
    OPTIONS allotments for queue  0 is   1
    OPTIONS quantum length for queue  0 is  12
    OPTIONS boost 0
    OPTIONS ioTime 1
    OPTIONS stayAfterIO True
    OPTIONS iobump True
    
    
    For each job, three defining characteristics are given:
    startTime : at what time does the job enter the system
    runTime   : the total CPU time needed by the job to finish
    ioFreq    : every ioFreq time units, the job issues an I/O
                (the I/O takes ioTime units to complete)
    
    Job List:
    Job  0: startTime   0 - runTime  10 - ioFreq   3
    Job  1: startTime   0 - runTime  15 - ioFreq   0
    
    
    Execution Trace:
    
    [ time 0 ] JOB BEGINS by JOB 0
    [ time 0 ] JOB BEGINS by JOB 1
    [ time 0 ] Run JOB 0 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 9 (of 10) ]
    [ time 1 ] Run JOB 0 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 8 (of 10) ]
    [ time 2 ] Run JOB 0 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 7 (of 10) ]
    [ time 3 ] IO_START by JOB 0
    IO DONE
    [ time 3 ] Run JOB 1 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 14 (of 15) ]
    [ time 4 ] IO_DONE by JOB 0
    [ time 4 ] Run JOB 0 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 6 (of 10) ]
    [ time 5 ] Run JOB 0 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 5 (of 10) ]
    [ time 6 ] Run JOB 0 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 4 (of 10) ]
    [ time 7 ] IO_START by JOB 0
    IO DONE
    [ time 7 ] Run JOB 1 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 13 (of 15) ]
    [ time 8 ] IO_DONE by JOB 0
    [ time 8 ] Run JOB 0 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 3 (of 10) ]
    [ time 9 ] Run JOB 0 at PRIORITY 2 [ TICKS 2 ALLOT 1 TIME 2 (of 10) ]
    [ time 10 ] Run JOB 0 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 1 (of 10) ]
    [ time 11 ] IO_START by JOB 0
    IO DONE
    [ time 11 ] Run JOB 1 at PRIORITY 2 [ TICKS 1 ALLOT 1 TIME 12 (of 15) ]
    [ time 12 ] IO_DONE by JOB 0
    [ time 12 ] Run JOB 0 at PRIORITY 2 [ TICKS 3 ALLOT 1 TIME 0 (of 10) ]
    [ time 13 ] FINISHED JOB 0
    [ time 13 ] Run JOB 1 at PRIORITY 2 [ TICKS 0 ALLOT 1 TIME 11 (of 15) ]
    [ time 14 ] Run JOB 1 at PRIORITY 1 [ TICKS 7 ALLOT 1 TIME 10 (of 15) ]
    [ time 15 ] Run JOB 1 at PRIORITY 1 [ TICKS 6 ALLOT 1 TIME 9 (of 15) ]
    [ time 16 ] Run JOB 1 at PRIORITY 1 [ TICKS 5 ALLOT 1 TIME 8 (of 15) ]
    [ time 17 ] Run JOB 1 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 7 (of 15) ]
    [ time 18 ] Run JOB 1 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 6 (of 15) ]
    [ time 19 ] Run JOB 1 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 5 (of 15) ]
    [ time 20 ] Run JOB 1 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 4 (of 15) ]
    [ time 21 ] Run JOB 1 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 3 (of 15) ]
    [ time 22 ] Run JOB 1 at PRIORITY 0 [ TICKS 11 ALLOT 1 TIME 2 (of 15) ]
    [ time 23 ] Run JOB 1 at PRIORITY 0 [ TICKS 10 ALLOT 1 TIME 1 (of 15) ]
    [ time 24 ] Run JOB 1 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 0 (of 15) ]
    [ time 25 ] FINISHED JOB 1
    
    Final statistics:
    Job  0: startTime   0 - response   0 - turnaround  13
    Job  1: startTime   0 - response   3 - turnaround  25
    
    Avg  1: startTime n/a - response 1.50 - turnaround 19.00
    ```

    可以看到， job 0 始终位于最高优先级的队列中。在 time 0 到 time 11 的这一段时间（后面 job 0 没有执行完给定时间片长度就结束了）， job 0 始终维持着 75% 的占用率。要维持 99% 的占用率，就是

    ```bash
    ./mlfq.py -l 0,1000,99:0,1000,0 -q 100 -i 1 -I -S
    ```

5. Given a system with a quantum length of 10 ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single longrunning (and potentially-starving) job gets at least 5% of the CPU? 

    最坏情况是该任务只在最高优先级的队列中执行了一次，也就是运行时长 10ms ，就降级到更低优先级的队列中，且最高优先级队列的 CPU 占用达到了 100% ，低优先级队列只能等待。为了让该任务的 CPU utilization 达到至少 5% ，需要的 priority boost time 最多为 $\frac{10}{0.05}=200ms$ 。

6. One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.

    `-I` flag 的作用是让完成 I/O 的任务回到队列的开头优先执行。无论 I/O 完成之前在执行哪个任务，之后开始执行的都会变成完成 I/O 的那个。