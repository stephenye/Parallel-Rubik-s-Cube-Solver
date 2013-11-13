Prallelized version of a C++ based Rubik's Cube Solver. Developed during the ECE1755 course. The repo was forked here for public accessing purpose.

Original solver tried 24 starting "orientations" to solve the cube, in a highly sequential manner. We try to seperate methods and attributes, and initiate multiple threads using various parallel programming tools, such as OpenMP, Pthreads, STM, and at the same time avoid dead-locks. More than 200% speedup can be achieved in experiments. 
