# vm-api-solution

## Code

1. First, write a simple program called `null.c` that creates a pointer to an integer, sets it to `NULL`, and then tries to dereference it. Compile this into an executable called `null`. What happens when you run this program?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int a = 1;
     int *ptr = &a;
     ptr = NULL;
     free(ptr);
     return 0;
   }
   ```

   没有报错。我认为这是一个 undefined behavior ，可能会 segmentation fault ，但不是一定会报错。我看别人的答案都是加了一个 `printf()` ，那肯定是更容易报错的。但是我这样是没有的。 

2. Next, compile this program with symbol information included (with the `-g` flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing `gdb null` and then, once gdb is running, typing `run`. What does gdb show you?

   正常退出。

3. Finally, use the valgrind tool on this program. We’ll use the `memcheck` tool that is a part of valgrind to analyze what happens. Run this by typing in the following: `valgrind --leak-check=yes ./null`. What happens when you run this? Can you interpret the output from the tool?

   这个程序 valgrind 也查不出内存泄露。

   ```
   ==4147== Memcheck, a memory error detector
   ==4147== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==4147== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
   ==4147== Command: ./null
   ==4147== 
   ==4147== 
   ==4147== HEAP SUMMARY:
   ==4147==     in use at exit: 0 bytes in 0 blocks
   ==4147==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
   ==4147== 
   ==4147== All heap blocks were freed -- no leaks are possible
   ==4147== 
   ==4147== For lists of detected and suppressed errors, rerun with: -s
   ==4147== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
   ```

4. Write a simple program that allocates memory using `malloc()` but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the --leak-check=yes flag)?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int *ptr = (int *)malloc(5 * sizeof(int));
     *ptr = 1;
     return 0;
   }
   ```

   gdb 运行不报错。valgrind 检查到内存泄露。

   ```
   ==4521== Memcheck, a memory error detector
   ==4521== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==4521== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
   ==4521== Command: ./null
   ==4521== 
   ==4521== 
   ==4521== HEAP SUMMARY:
   ==4521==     in use at exit: 20 bytes in 1 blocks
   ==4521==   total heap usage: 1 allocs, 0 frees, 20 bytes allocated
   ==4521== 
   ==4521== 20 bytes in 1 blocks are definitely lost in loss record 1 of 1
   ==4521==    at 0x4841888: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==4521==    by 0x10914A: main (in /home/somebody/null)
   ==4521== 
   ==4521== LEAK SUMMARY:
   ==4521==    definitely lost: 20 bytes in 1 blocks
   ==4521==    indirectly lost: 0 bytes in 0 blocks
   ==4521==      possibly lost: 0 bytes in 0 blocks
   ==4521==    still reachable: 0 bytes in 0 blocks
   ==4521==         suppressed: 0 bytes in 0 blocks
   ==4521== 
   ==4521== For lists of detected and suppressed errors, rerun with: -s
   ==4521== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
   ```

5. Write a program that creates an array of integers called `data` of size 100 using `malloc`; then, set `data[100]` to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int *ptr = (int *)malloc(100 * sizeof(int));
     ptr[100] = 0;
     return 0;
   }
   ```

   gdb 运行不报错。valgrind 产生错误。

   ```
   ==4953== Memcheck, a memory error detector
   ==4953== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==4953== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
   ==4953== Command: ./null
   ==4953== 
   ==4953== Invalid write of size 4
   ==4953==    at 0x109159: main (null.c:7)
   ==4953==  Address 0x4a6a1d0 is 0 bytes after a block of size 400 alloc'd
   ==4953==    at 0x4841888: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==4953==    by 0x10914A: main (null.c:6)
   ==4953== 
   ==4953== 
   ==4953== HEAP SUMMARY:
   ==4953==     in use at exit: 400 bytes in 1 blocks
   ==4953==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated
   ==4953== 
   ==4953== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
   ==4953==    at 0x4841888: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==4953==    by 0x10914A: main (null.c:6)
   ==4953== 
   ==4953== LEAK SUMMARY:
   ==4953==    definitely lost: 400 bytes in 1 blocks
   ==4953==    indirectly lost: 0 bytes in 0 blocks
   ==4953==      possibly lost: 0 bytes in 0 blocks
   ==4953==    still reachable: 0 bytes in 0 blocks
   ==4953==         suppressed: 0 bytes in 0 blocks
   ==4953== 
   ==4953== For lists of detected and suppressed errors, rerun with: -s
   ==4953== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
   ```

6. Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int *ptr = (int *)malloc(100 * sizeof(int));
     *ptr = 0;
     *(ptr + 1) = 1;
     printf("%d\n", *(ptr + 1));
     free(ptr);
     printf("%d\n", *(ptr + 1));
     return 0;
   }
   ```

   可以运行，但是明显内存泄露会出现问题，前一个输出 `1` ，后一个输出 `5` 。valgrind 报错。

   ```
   ==5529== Memcheck, a memory error detector
   ==5529== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
   ==5529== Using Valgrind-3.19.0 and LibVEX; rerun with -h for copyright info
   ==5529== Command: ./null
   ==5529== 
   1
   ==5529== Invalid read of size 4
   ==5529==    at 0x1091BB: main (null.c:11)
   ==5529==  Address 0x4a6a044 is 4 bytes inside a block of size 400 free'd
   ==5529==    at 0x484426F: free (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==5529==    by 0x1091B2: main (null.c:10)
   ==5529==  Block was alloc'd at
   ==5529==    at 0x4841888: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
   ==5529==    by 0x10916A: main (null.c:6)
   ==5529== 
   1
   ==5529== 
   ==5529== HEAP SUMMARY:
   ==5529==     in use at exit: 0 bytes in 0 blocks
   ==5529==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
   ==5529== 
   ==5529== All heap blocks were freed -- no leaks are possible
   ==5529== 
   ==5529== For lists of detected and suppressed errors, rerun with: -s
   ==5529== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
   ```

7. Now pass a funny value to `free` (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   
   int main() {
     int *ptr = (int *)malloc(100 * sizeof(int));
     *ptr = 0;
     *(ptr + 1) = 1;
     printf("%d\n", *(ptr + 1));
     free(ptr + 10);
     printf("%d\n", *(ptr + 1));
     return 0;
   }
   ```

   在编译的时候就会出现 warning 

   ```
   null.c: In function ‘main’:
   null.c:10:3: warning: ‘free’ called on pointer ‘ptr’ with nonzero offset 40 [-Wfree-nonheap-object]
      10 |   free(ptr + 10);
         |   ^~~~~~~~~~~~~~
   null.c:6:21: note: returned from ‘malloc’
       6 |   int *ptr = (int *)malloc(100 * sizeof(int));
         |                     ^~~~~~~~~~~~~~~~~~~~~~~~~
   ```

8. Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use `realloc()` to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use `realloc()` to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs.

   动态数组的性能对比： https://en.wikipedia.org/wiki/Dynamic_array#Performance

9. Spend more time and read about using gdb and valgrind. Knowing your tools is critical; spend the time and learn how to become an expert debugger in the UNIX and C environment.