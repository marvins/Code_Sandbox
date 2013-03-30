import List

-- Check the difference between the numbers
same_digits n1 n2 = (show n1) \\ (show n2) == []

-- make sure that all items in the list meet the predicate
check n = all ( same_digits n) (map (n*) [2..6])

problem_52 = head $ filter check [1..]
