task a to c are done maintaining all the keypoints.

CG algorithm is implemented but D and D inverse is constructed
explictly. An algorithm for nonSPD_CG is written where D should
will be automatically generated, but it has some unfixed bug and
the result is not correct. 
Task e is implemented and a log plot is generated using task_e_logplot.py.

It is clear that after t=3, it somewhat becomes constant.
Task f is completed where the given value are loaded. 

Task h is done and on a 4 processor, for 8X8 matrix
the code is tested. Local bondaries are also included.

Task i has implemented but there are some bugs. The D matrix should
be 32X32 at local, for a system where N1 = 8, N2 = 8. But locally
it is a 16X16 matrix and also there are some missing values.


