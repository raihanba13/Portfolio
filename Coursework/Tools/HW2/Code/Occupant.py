class Occupant:
    def __init__(self, familyname, givenname, room):
        
        self.familyname = familyname
        self.givenname = givenname
        self.room = room
        
    def __repr__(self):
       return f"Instructor name: {self.familyname}, {self.givenname}; {self.room}"