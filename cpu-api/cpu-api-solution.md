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