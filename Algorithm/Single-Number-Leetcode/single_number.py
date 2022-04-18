class Solution:
    def singleNumber(self, nums: List[int]) -> List[int]:


        result = [0]*2

        ab = 0 

        for i in nums:
            ab ^= i
            
        ab &= -ab

        for i in nums:
            if (ab & i) == 0:
                result[0] ^= i
            else:
                result[1] ^= i
                
        return result
        
