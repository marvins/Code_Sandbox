reverseNum = read . reverse . show

-- Convert the number to string, compare with its reverse
palindrome x =
   sx == reverse sx
   where
   sx = show x 


lychrel = 
   not . any palindrome . take 50 . tail . iterate next
   where
   next x = x + reverseNum x
  

problem_55 = length $ filter lychrel [1..10000]
