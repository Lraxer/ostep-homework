# Homework

## Simulation

1. Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.

   以 seed 为 2 为例

   ```shell
   python lottery.py -j 3 -s 2 -c
   ```

2. Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., -l 10:1,10:100). What happens when the number of tickets is so imbalanced? Will job 0 ever run before job 1 completes? How often? In general, what does such a ticket imbalance do to the behavior of lottery scheduling?

   当 tickets 非常不平衡的时候，job 0 在 job 1 运行结束前基本没有机会运行。这会导致 job 0 长期处于饥饿状态。

3. When running with two jobs of length 100 and equal ticket allocations of 100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.

   略。

4. How does your answer to the previous question change as the quantum size (-q) gets larger?

   越来越不公平。

5. Can you make a version of the graph that is found in the chapter? What else would be worth exploring? How would the graph look with a stride scheduler?

   随着 job length 的增加，公平性会越来越接近 1 。 stride scheduler 比 lottery scheduler 接近的速度更快。
