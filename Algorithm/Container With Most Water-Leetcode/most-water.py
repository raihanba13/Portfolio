class Solution:
    def maxArea(self, height: List[int]) -> int:
        
        max_area = 0
        
        
        for count, i in enumerate(height, start=1):
            width = len(height) - count
            for j in height[::-1]:
                
                if i < j:
                    t_max_area = i*width
                else:
                    t_max_area = j*width
                    
                if t_max_area > max_area:
                    max_area = t_max_area
                    
                width -= 1
                
                if width == 0:
                    break
                
                
        return int(max_area)
                
                