# cpu-sched-multi-solution

## Simulation

1. To start things off, let’s learn how to use the simulator to study how to build an effective multi-processor scheduler. The first simulation will run just one job, which has a run-time of 30, and a working-set size of 200. Run this job (called job ’a’ here) on one simulated CPU as follows: `./multi.py -n 1 -L a:30:200` . How long will it take to complete? Turn on the `-c` flag to see a final answer, and the `-t` flag to see a tick-by-tick trace of the job and how it is scheduled.

   需要 30 运行时间。由于默认的 cache 是 100 ，小于需要的 cache 大小 200 ，因此没办法 warm up 。运行时间和 run time 相等。

2. Now increase the cache size so as to make the job’s working set (size=200) fit into the cache (which, by default, is size=100); for example, run `./multi.py -n 1 -L a:30:200 -M 300` . Can you predict how fast the job will run once it fits in cache? (hint: remember the key parameter of the warm rate, which is set by the `-r` flag) Check your answer by running with the solve flag (`-c`) enabled.

   运行时间是 20 。cache 大小足够，可以 warm up ，默认需要 10 单位时间 warm up，默认的 warmup rate 是 2x 的速度，所以运行时间是 `10+20/2=20` 。

3. One cool thing about multi.py is that you can see more detail about what is going on with different tracing flags. Run the same simulation as above, but this time with time left tracing enabled (`-T`). This flag shows both the job that was scheduled on a CPU at each time step, as well as how much run-time that job has left after each tick has run. What do you notice about how that second column decreases?

   ```shell
   ./multi.py -n 1 -L a:30:200 -M 300 -cT
   ```

   前 10 单位时间每个 step `-1` ，后 10 单位时间每个 step `-2` 。理由在第二题说了。

4. Now add one more bit of tracing, to show the status of each CPU cache for each job, with the `-C` flag. For each job, each cache will either show a blank space (if the cache is cold for that job) or a ’w’ (if the cache is warm for that job). At what point does the cache become warm for job ’a’ in this simple example? What happens as you change the warmup time parameter (`-w`) to lower or higher values than the default?

   ```shell
   ./multi.py -n 1 -L a:30:200 -M 300 -Cc
   ```

   结果是

   ```
   Scheduler central queue: ['a']
   
      0   a cache[ ]     
      1   a cache[ ]     
      2   a cache[ ]     
      3   a cache[ ]     
      4   a cache[ ]     
      5   a cache[ ]     
      6   a cache[ ]     
      7   a cache[ ]     
      8   a cache[ ]     
      9   a cache[w]     
   -------------------
     10   a cache[w]     
     11   a cache[w]     
     12   a cache[w]     
     13   a cache[w]     
     14   a cache[w]     
     15   a cache[w]     
     16   a cache[w]     
     17   a cache[w]     
     18   a cache[w]     
     19   a cache[w]
   ```

   warmup time 设为 `x` ，那从 `x-1` 开始就会被标记为 `w` 。

5. At this point, you should have a good idea of how the simulator works for a single job running on a single CPU. But hey, isn’t this a multi-processor CPU scheduling chapter? Oh yeah! So let’s start working with multiple jobs. Specifically, let’s run the following three jobs on a two-CPU system (i.e., type `./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 `) Can you predict how long this will take, given a round-robin centralized scheduler? Use `-c` to see if you were right, and then dive down into details with `-t` to see a step-by-step and then `-C` to see whether caches got warmed effectively for these jobs. What do you notice?

   需要时间 150 。cache warmup time 和时间片长度一样大，所以缓存没有起到加速作用。`(100+100+100)/3=150` 。

6. Now we’ll apply some explicit controls to study **cache affinity**, as described in the chapter. To do this, you’ll need the `-A` flag. This flag can be used to limit which CPUs the scheduler can place a particular job upon. In this case, let’s use it to place jobs ’b’ and ’c’ on CPU 1, while restricting ’a’ to CPU 0. This magic is accomplished by typing this `./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 -A a:0,b:1,c:1` ; don’t forget to turn on various tracing options to see what is really happening! Can you predict how fast this version will run? Why does it do better? Will other combinations of ’a’, ’b’, and ’c’ onto the two processors run faster or slower?

   任务 a 需要 55 单位时间，b 和 c 在 cpu 1 上一共运行了 110 单位时间，各 55 单位时间。第 5 题里面运行顺序是 `a->c->b->a` 这样，每次 b, c warmup 以后，cache 又被 a 都占据了。现在 cpu 1 上只运行 b, c ，cache 的大小足够 warmup 这两个任务，因此各运行一个时间片，也就是 20 单位时间后，就以 2 倍速运行，`10+10+90/2*2=110` 。

   其他组合不会运行得更快。因为只有 b, c 能同时被 100 的 cache 容纳。

7. One interesting aspect of caching multiprocessors is the opportunity for better-than-expected speed up of jobs when using multiple CPUs (and their caches) as compared to running jobs on a single processor. Specifically, when you run on N CPUs, sometimes you can speed up by more than a factor of N , a situation entitled super-linear speedup. To experiment with this, use the job description here (`-L a:100:100,b:100:100,c:100:100`) with a small cache (-M 50) to create three jobs. Run this on systems with 1, 2, and 3 CPUs (`-n 1, -n 2, -n 3`). Now, do the same, but with a larger per-CPU cache of size 100. What do you notice about performance as the number of CPUs scales? Use -c to confirm your guesses, and other tracing flags to dive even deeper.

   ```shell
   ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -M 100 -CTc
   ```

   当 cache 大小是 100 的时候，使用 `-n 1` 时，结果是 300 单位时间，`-n 2` 的时候，结果是 150 单位时间，`-n 3` 则是 55 。这就是因为 cache affinity 。

8. One other aspect of the simulator worth studying is the per-CPU scheduling option, the `-p` flag. Run with two CPUs again, and this three job configuration (`-L a:100:100,b:100:50,c:100:50`). How does this option do, as opposed to the hand-controlled affinity limits you put in place above? How does performance change as you alter the ’peek interval’ (`-P`) to lower or higher values? How does this per-CPU approach work as the number of CPUs scales?

   ```shell
   ./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 -p -CTc
   ```

   不加 `-p` 的时候，两个 CPU 会轮流运行 a, b, c 三个任务。如果加上 `-A a:0,b:1,c:0` ，则是 a, c 使用 CPU 0 ，b 使用 CPU 1 。使用 `-p` 参数，会把 a, c 分配在 CPU 0 运行，b 在 CPU 1 运行。当 b 提前运行完成后，就会再分配 a 到 CPU 1 运行，最终只需要 100 单位时间即可运行完成，充分利用了 CPU 资源。

   `-P` 代表 peek interval 。默认的 peek interval 是 30 。一般来说这个数值越小，两个 CPU 的负载就会越均衡，但是考虑到 cache affinity ，什么时候更快是不好说的。

   当 CPU 的数量超过任务数量的时候，per-CPU approach 的性能就未必会高了。例如 `-n 4` 运行时间是 60 ，`-n 3` 是 55 。

9. Finally, feel free to just generate random workloads and see if you can predict their performance on different numbers of processors, cache sizes, and scheduling options. If you do this, you’ll soon be a multi-processor scheduling master, which is a pretty awesome thing to be. Good luck!
