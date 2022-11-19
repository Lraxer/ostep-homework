# vm-segmentation-solution

## Simulation

1. First let’s use a tiny address space to translate some addresses. Here’s a simple set of parameters with a few different random seeds; can you translate the addresses?

   ```shell
   segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0 
   segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 
   segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2
   ```

   以第一个为例：

   ```
   ARG seed 0
   ARG address space size 128
   ARG phys mem size 512
   
   Segment register information:
   
     Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
     Segment 0 limit                  : 20
   
     Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
     Segment 1 limit                  : 20
   
   Virtual Address Trace
     VA  0: 0x0000006c (decimal:  108) --> PA or segmentation violation?
     VA  1: 0x00000061 (decimal:   97) --> PA or segmentation violation?
     VA  2: 0x00000035 (decimal:   53) --> PA or segmentation violation?
     VA  3: 0x00000021 (decimal:   33) --> PA or segmentation violation?
     VA  4: 0x00000041 (decimal:   65) --> PA or segmentation violation?
   ```

   判断 VA 是否在 segment 0 中，只需要直接比较 VA 是否小于等于 limit ；判断是否在 VA 1 中，需要比较
   $$
   |\mathrm{VA}-\mathrm{address\ space\ size}| \le \mathrm{segment\ 1\ limit}
   $$
   显然只有 VA 0 在 segment 1 中。其他都是非法地址。

2. Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). What is the highest legal virtual address in segment 0? What about the lowest legal virtual address in segment 1? What are the lowest and highest illegal addresses in this entire address space? Finally, how would you run `segmentation.py` with the `-A` flag to test if you are right?

   使用上一题的参数。单位是十进制。

   segment 0 的 highest legal VA: 19 。segment 1 的 lowest legal VA: 108 。

   lowest and highest illegal addresses in address space (VA): 20, 107 。

3. Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: valid, valid, violation, ..., violation, valid, valid? Assume the following parameters:

   ```shell
   segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 ? --l0 ? --b1 ? --l1 ?
   ```

   也就是只有 `0, 1, 14, 15` 是 valid 。首先可以确定 `--b0 0 --l0 2` ，b1 是什么其实并不影响结果 。然后根据第一题回答的公式可以计算出结果 `--l1 2` 。因此结果是

   ```shell
   segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 2 --b1 16 --l1 2
   ```

4. Assume we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (not segmentation violations). How should you configure the simulator to do so? Which parameters are important to getting this outcome?

   可以认为随机生成的虚拟地址是均匀的，然后只需要把 limit 设置好就可以了。例如虚拟地址 1KB ，物理地址 16KB ，则地址总数是 `1024` 个，`1024*0.9*0.5=460.8` ，取整得到 `461` 。也就是每个段平均需要包含这么多的地址数量。

   ```shell
   ./segmentation.py -a 1k -p 16k --b0 0 --l0 461 --b1 16384 --l1 461
   ```

5. Can you run the simulator such that no virtual addresses are valid? How?

   limit 设为 `0` 。

   ```shell
   ./segmentation.py -a 1k -p 16k --b0 0 --l0 0 --b1 16384 --l1 0 -c
   ```