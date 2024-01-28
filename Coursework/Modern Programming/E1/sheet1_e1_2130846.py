

def sort_array(i_list):
    for m in range(len(i_list)):
        minimum_idx = m 

        for n in range(m+1, len(i_list)):
            if i_list[minimum_idx] > i_list[n]:
                minimum_idx = n

        i_list[minimum_idx], i_list[m] = i_list[m], i_list[minimum_idx]

    return i_list

# input array
while True:
    try:
        lenght_of_input = int(input('Enter n(count of number):'))
        break
    except:
        print('Wrong input. Try again!')


i_list = []
member_count = 1

while lenght_of_input:
    try:
        x = input(f'{member_count}th int: ')
        int(x)
        i_list.append(int(x))
        lenght_of_input -= 1
        member_count += 1
    except Exception as e:
        print(e)
        print('Please enter a valid int.')

result_list = sort_array(i_list)

print ("\nOutput:\n")
print(result_list)