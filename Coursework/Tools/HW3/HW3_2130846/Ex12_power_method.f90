program main 

    implicit none
    double precision, dimension(3) :: data = (/ 4d0, -1d0, -2d0 /)
    double precision, allocatable :: t_matrix(:, :)
    integer :: n, i, j, k

    print *, "Enter n: "
    read *, n

    allocate(t_matrix(n, n))
    t_matrix = 0d0

    ! create the matrix
    do i = 1, n 
        do j = 1, 3, 1
            t_matrix(i, i+j-1) = data(j)    
        end do

        k = 2
        do j = i-1, 1, -1
            t_matrix(i, j) = data(k)
            k = k + 1
            if (k > 3) then
                exit
            end if
        end do
    end do

    call matrix_output(t_matrix, n)
    write (*, "(A, F12.5)") "Largest absolute Eigen Value: ", power_method(t_matrix, n)

contains
    subroutine matrix_output(matrix, n)
        double precision, dimension(n, n), intent(in) :: matrix
        integer, intent(in) :: n 

        do i = 1, n 
            do j = 1, n 
                ! print *, i, ", ", j, " : ", matrix(i, j)
                write (*, "(F8.2A)", advance="no")   matrix(i, j), " "
            end do
            write (*, "(A)") 
        end do
    end subroutine matrix_output

    function power_method(matrix, n) result(res)
        integer, intent(in) :: n
        double precision, dimension(n, n), intent(in) :: matrix
        double precision, dimension(n, 1) :: x, y
        double precision :: res
        double precision :: tol = 1d0
        y = 1d0

        do while (tol > 10d-12)
            x = y
            y = matmul(matrix, x)/norm2(matmul(matrix, x))
            tol = norm2(y - x)
        end do
        res = norm2(matmul(matrix, y))

    end function power_method


end program main