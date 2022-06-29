# Homework

## Simulation

1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.

    ### FIFO

    Response time: $\frac{0+200+400}{3}=200$

    Turnaround time: $\frac{200+400+600}{3} = 400$

    ### SJF

    Response time: $\frac{0+200+400}{3}=200$

    Turnaround time: $\frac{200+400+600}{3} = 400$

    由于三个作业长度都是 200 ，因此 FIFO 和 SJF 在这两个测度上没有体现出区别。

2. Now do the same but with jobs of different lengths: 100, 200, and 300. 

    ### FIFO

    Response time: $\frac{0+100+300}{3} \approx 133.33$

    Turnaround time: $\frac{100+300+600}{3} \approx 333.33$

    ### SJF

    Response time: $\frac{0+100+300}{3} \approx 133.33$

    Turnaround time: $\frac{100+300+600}{3} \approx 333.33$

    三个任务是本身就是按从短作业到长作业运行的，因此也没有区别。

3. Now do the same, but also with the RR scheduler and a time-slice of 1. 

    ### RR

    Response time: $\frac{0+1+2}{3} = 1$

    Turnaround time: $\frac{298+499+600}{3} \approx 465.67$

    这里要特别注意每一个 job 的完成时间。

4. For what types of workloads does SJF deliver the same turnaround times as FIFO? 

    按 job 的长度由短到长依次执行，且执行过程不中断。

5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR? 

    按 job 的长度由短到长依次执行，Round-Robin 的时间片长度大于等于第二长的 job 的长度。

6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend? 

    Response time 会增加。

7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?

    Response time 会增加。

    Response time 的最坏情况：第 $N$ 个 job 的 $response\ time = (N-1)q$ 。
    
    $Average\ response\ time = \frac{\sum^{N-1}_{i=0}i \times q}{N}=\frac{(N-1)q}{2}$