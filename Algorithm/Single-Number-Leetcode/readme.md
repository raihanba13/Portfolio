## Single Number III

# Problem Statement 
A list of int is given where exactly two elements are given once and other elements are given exactly twice. We need to find the two number that occurs only one.

# Solution 
We will use the concept of XOR. This should be noted that, 
1. XOR of zero and some bit returns that bit, p^0=p
2. XOR of two same bit returns 0, p^p=0
3. p\^q^p can be written as (p\^p)^q=q

Now if we loop through all the numbers and imagine p and q are our unique numbers, we will 
have p^q, i.e.
    p\^r\^q\^r = p\^(r\^r)\^q = p\^q 
    
Now we need to find the last significant bit, we can simply do it by
    m = p^q AND -(p^q)
This will help us to divide the numbers into two group, where each group will have one unique solution.
Now we will loop through the number again and if **m AND num** gives us 0, it means the significant bit is different, we will keep doing XOR on this group and this will give us the first result. If it is not zero, that group will give us the second result.