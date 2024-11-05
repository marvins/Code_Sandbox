#  Memory Tracker

This is a dumb little project to help find memory-leak style problems in a codebase. 

The idea is that by instrumenting your classes, you can track real-time memory usage in an efficient and practical manner.  This project is designed for runtime systems where a lot of heavy-handed GDB or Valgrind style choices are not available to you. 

## Conditions

This project is designed for conditions where you have a problem with memory usage and cannot figure out why.  Specifically it happens over the following conditions:

- It takes 6+ hours for a condition to manifest itself.  
    * This will make it challenging to use print statements and other hacks.  The growth rate is hard to track.
- The performance of a system requires you run in a release build.
    * This makes it impossible to use normal tools. Valgrind and memcheck don't operate.

## Things to Track

1. Memory usage by class:
    - We should be able to know the following:
        - How much of a particular type is actively created?
        - What is the average amount of memory allocated for that type?
        - What is the average deallocation rate?

2. Memory usage of full application
    - What is current RAM utilization?
    - What is RAM growth rate?

3. 