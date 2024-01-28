! polynomials and Newton's method
! author:  Holger Arndt
! version: 10.05.2022
! framework for sheet 3, exercise 11

program main

  implicit none

  double precision, dimension(5) :: p0 = (/ 0, 0, 0, 0, 0 /)
  double precision, dimension(5) :: p1 = (/ 2, -3, -5, 4, 0 /)
  double precision, dimension(5) :: p2 = (/ 1, 1, 1, 0, 0 /)
  double precision, dimension(5) :: p3 = (/ -1, -1, -1, 0, 0 /)
  double precision, dimension(5) :: p4 = (/ -2, -1, 1, 0, 0 /)
  double precision :: x

  write (*, "(A)", advance = "no") 'p0(x) = '
  call polyOutput(p0)
  write (*, "(A)", advance = "no") 'p1(x) = '
  call polyOutput(p1)
  write (*, "(A)", advance = "no") 'p2(x) = '
  call polyOutput(p2)
  write (*, "(A)", advance = "no") 'p3(x) = '
  call polyOutput(p3)
  write (*, "(A)", advance = "no") 'p4(x) = '
  call polyOutput(p4)

  ! uncomment next two lines for part a)
  write (*, "(A, G0.3)") "p2(1) = ", polyEval(p2, 1d0)
  write (*, "(A, G0.3)") "p1(-2) = ", polyEval(p1, -2d0)

  ! uncomment next four lines for part b)
  write (*, "(A)", advance = "no") "p0'(x) = "
  call polyOutput(polyDeriv(p0))
  write (*, "(A)", advance = "no") "p1'(x) = "
  call polyOutput(polyDeriv(p1))

  ! uncomment next seven lines for part c)
  write (*, "(A, G0.3)") "newton(p4, 0) = ", newton(p4, 0d0)
  write (*, "(A, G0.3)") "newton(p4, 1) = ", newton(p4, 1d0)
  x = newton(p1, 2d0)
  write (*, "(A, G0.6)") "x = newton(p1, 2) = ", x
  write (*, "(A, 1PG0.3)") "p1(x) = ", polyEval(p1, x)
  write (*, "(A)", advance = "no") "try newton(p3, 2):"
  x = newton(p3, 2d0)

contains
  subroutine polyOutput(p)
    double precision, dimension(5), intent(in) :: p
    integer :: i, degree
    degree = 4
    do while (degree >= 0 .and. p(degree + 1) == 0)
       degree = degree - 1
    end do
    if (degree == -1) then
       print "(G0.3)", 0d0
    else
       do i = degree, 0, -1
          if (i < degree .and. p(i + 1) > 0) then
             write (*, "(A)", advance = "no") '+'
          else if(p(i + 1) < 0) then
             write (*, "(A)", advance = "no") '-'
          end if
          if (p(i + 1) /= 0 .and. (abs(p(i + 1)) /= 1 .or. i == 0)) &
               write (*, "(G0.3)", advance = "no") abs(p(i + 1))
          if (p(i + 1) /= 0 .and. i > 0) &
               write (*, "(A)", advance = "no") 'x'
          if (p(i + 1) /= 0 .and. i > 1) &
               write (*, "(A, I1)", advance = "no") '^', i
       end do
       print *
    end if
  end subroutine polyOutput

  ! add your three functions here

   double precision FUNCTION polyEval(p, x) result(result)
   IMPLICIT NONE
   double precision, dimension(1:), intent(in)  :: p ! input
   double precision, intent(in) :: x

   integer :: i

   result = p(5)

   do i = 4, 1, -1
      result = result*x + p(i)
   end do

  end FUNCTION polyEval

  FUNCTION polyDeriv(p) result(p_dash)
   IMPLICIT NONE
   double precision, dimension(1:), intent(in)  :: p ! input
   double precision, dimension(4) :: p_dash ! output

   integer :: i

   do i = 2, 5, 1
      p_dash(i-1) = p(i)*(i-1)
   end do

  end FUNCTION polyDeriv

  REAL FUNCTION newton(p, x0) result(root)
   IMPLICIT NONE
   double precision, dimension(1:), intent(in)  :: p ! input
   double precision, intent(in) :: x0
   double precision :: t_x0

   integer :: i
   t_x0 = x0

   do i = 0, 100
      root = t_x0 - polyEval(p, t_x0)/polyEval(polyDeriv(p), t_x0)

      if (ABS(t_x0-root) < 10d-14) then
         return
      end if

      t_x0 = root
   end do

  end FUNCTION newton


end program main
