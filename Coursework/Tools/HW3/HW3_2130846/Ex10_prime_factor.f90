program prime_factor
    implicit none
    integer :: i
    integer :: div = 2
    integer :: t_i
    print *, "enter an integers"
    read *, i
    t_i = i

    do 10 while (div <= i)
        if (mod(t_i, div) == 0) then
            t_i = t_i/div
            write (*, "(I6)", advance="no") div
            ! cycle can also be used
            goto 10
        end if

        div = div + 1
    10 end do

end program prime_factor
