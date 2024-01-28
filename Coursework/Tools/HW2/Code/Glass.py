class Glass:
    def __init__(self, volume=250, content=0):
        
        self.volume = volume
        self.content = content
        
        if volume < content:
            raise ValueError('Over flooded!')
        
    def fillIn(self, contentToAdd):
        
        if self.volume < (self.content+contentToAdd):
            raise ValueError('Over flooded!')
        
        self.content += contentToAdd
        
    def drink(self, contentToDrink):
        
        if self.content < contentToDrink:
            raise ValueError('Not enough to drink!')
        
        self.content -= contentToDrink
        
    def __repr__(self):
       return f"Glass with volume {self.volume} unit and content {self.content} unit"