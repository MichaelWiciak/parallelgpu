
No. Process:          Mean time (average of 3 runs)           Parallel speed-up, S:
============          =============================           =====================
1	                    0.019868                                1.0
2 	                  0.011297                                1.758696999203328
3 	                  0.007503                                2.648007463681194
4	                    0.005903                                3.365746230730137

Architecture that the timing runs were performed on: desktop or laptop; make; and number of CPU cores:
Laptop, MacBook Air M1 Chip, 8 (4 performance and 4 efficiency) 
A brief interpretation of these results (2-3 sentences should be sufficient):
As you increase the number of processes (at least until 4 for this example), the lower the mean time is. 
Meaning the program can be executed faster due to parallelisation. However, as seen by S, the parallel speed-up is not linear as you could imagine in perfect circumstances.
This is due to the parallel overhead associated with MPI.
These overheads can include message passing latency (like MPI_Send, MPI_Recv, and MPI_Reduce) and contention for resources, leading to diminishing returns in speed-up despite adding more processes. 
As the number of processes increases, so does the frequency of these communication operations, so the overheads become more significant and parallel speed-up increases at a slower rate (potentially could even decrease if we continued increasing the number of processes). 