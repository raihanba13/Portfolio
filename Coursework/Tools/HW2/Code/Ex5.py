from Glass import Glass

g1 = Glass(100, 36)

print('Glass 1:')
print(g1)

print('Glass 2:')
try:
    g2 = Glass(100, 360)
except Exception as e:
    print(e)

g3 = Glass(125, 32)

print('Glass 3:')
print(g3)

g3.fillIn(28)
print('Glass 3, adding 28 to content:')
print(g3)

print('Glass 3, adding 280 to content, this will raise error and will not be updated:')
try:
    g3.fillIn(280)
except Exception as e:
    print(e)
print(g3)

print('Glass 3, drinking 20:')
g3.drink(20)
print(g3)

print('Glass 3, drinking 60, this will throw error:')
try:
    g3.drink(60)
except Exception as e:
    print(e)
print(g3)
