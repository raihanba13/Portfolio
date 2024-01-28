class Room:
    def __init__(self, building, floor, number):
        
        self.building = building
        self.floor = floor
        self.number = number
        
    def __repr__(self):
       return f"Room: {self.building}.{self.floor}.{self.number}"