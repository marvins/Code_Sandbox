#  Memory Tracker

This is a dumb little project to help find memory-leak style problems in a codebase. 

The idea is that by instrumenting your classes, you can track real-time memory usage in an efficient and practical manner.  This project is designed for runtime systems where a lot of heavy-handed GDB or Valgrind style choices are not available to you. 

## Conditions

This project is designed for conditions where you have a problem with memory usage and cannot figure out why.  Specifically it happens over the following conditions:

- It takes 6+ hours for a condition to manifest itself.  
    * This makes 