 
input_int = int(input("Enter an int: "))
t_input = input_int
prime_divisor = []

div = 2

while div < t_input:
    if input_int % div == 0:
        prime_divisor.append(div)
        input_int /= div 
        continue
    
    div += 1
    
if len(prime_divisor) == 0:
    prime_divisor = [input_int]
    
print("Prime Divisor: ", prime_divisor)
