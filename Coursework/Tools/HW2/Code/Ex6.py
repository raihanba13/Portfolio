import re
from Occupant import Occupant
from Room import Room

input_f = open('fbc_math.txt', 'r')
regex = r"(.+)\s([A-Z]+)\.(\d+)\.(\d+)"
occupant_list = []

for x in input_f:
    
    txt_match = re.match(regex, x.strip())
    
    if txt_match:
        room = Room(txt_match.group(2), txt_match.group(3), txt_match.group(4))
        occ = Occupant(txt_match.group(1).split(",")[0].strip(), txt_match.group(1).split(",")[1].strip(), room)
        
        print(occ)
        print("=====================================")
     
    occupant_list.append(occ)   
    
    
import operator
sorted_x = sorted(occupant_list, key=operator.attrgetter('room.building'))

print("#########################")
for y in sorted_x:
    print(y)