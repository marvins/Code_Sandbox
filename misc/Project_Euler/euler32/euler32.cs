//pandigital products program
using System;
using System.Collections.Generic;

namespace Pandigital_Products
{
    /// <summary>
    /// This is a class designed to store nice generic functions which 
    /// will aid in the building of a workable solution. Functions tend
    /// to be more debugging oriented.
    /// </summary>
    class generic_functions
    {
        /// <summary>
        /// Converts an array into a string
        /// </summary>
        /// <param name="array">input item</param>
        /// <returns></returns>
        public static String array2string( int[] array )
        {
            String output = String.Empty;
            for (int i = 0; i < array.Length; i++)
                output += array[i].ToString();
            return output;
        }

        public static int array2num(int[] val, int id1, int id2)
        {
            int result = 0;
            int power = (id2-id1)-1;
            for (int i = id1; i < id2; i++)
                result += val[i] * (int)Math.Pow(10, power--);

            return result;
        }
    }
    /// <summary>
    /// Engine which given an input string
    /// will return a list of permutations.
    /// </summary>
    class Permute
    {
        private void swap(ref int a, ref int b)
        {
            if (a == b) return;
            a ^= b;
            b ^= a;
            a ^= b;
        }

        public List<int[]> setper(int[] list)
        {
            List<int[]> list_of_permutations = new List<int[]>();
            int x = list.Length - 1;
            go(list, 0, x, ref list_of_permutations);

            return list_of_permutations;
        }

        private void go(int[] list, int k, int m, ref List<int[]> list_of_permutations)
        {
            int i;
            if (k == m)
            {
                if (Main_Program.DEBUG == true)
                {
                    String output = generic_functions.array2string(list);
                    Console.WriteLine(output);
                }
                int[] intArr = new int[list.Length];
                list.CopyTo(intArr, 0);
                list_of_permutations.Add(intArr);

            }
            else
                for (i = k; i <= m; i++)
                {
                    swap(ref list[k], ref list[i]);
                    go(list, k + 1, m, ref list_of_permutations);
                    swap(ref list[k], ref list[i]);
                }
        }
    }

    class Main_Program
    {
        public const Boolean DEBUG = false;

        static void Main()
        {
            //initialize check table
            Boolean[] array_table = new Boolean[987654321];
            for (int b = 0; b < array_table.Length; b++ )
                array_table[b]= false;

            //create permutation engine
            Permute engine = new Permute();

            //create initial input string
            int [] array = new int[9];
            for (int i = 0; i < array.Length; i++)
                array[i] = i + 1;
            
            /*calling the permute function*/
            List<int[]> list_of_permutations = engine.setper(array);
            
            //iterate through each permutation
            int oper1, oper2, result;
            foreach (int[] item in list_of_permutations)
            {
                //iterate through each possible split
                //first product
                for (int i = 1; i < item.Length-2; i++)
                {
                    //compute the first item
                    oper1 = generic_functions.array2num(item, 0, i);

                    //second product
                    for (int j = i + 1; j < item.Length - 1; j++)
                    {
                        oper2 = generic_functions.array2num(item, i, j);

                        result = generic_functions.array2num(item, j, item.Length);

                        if (oper1 * oper2 == result)
                        {
                            if (array_table[result] == false)
                            {
                                Console.WriteLine(result.ToString() + " = SUCCESS");
                                array_table[result] = true;
                            }
                            else
                            {
                                Console.WriteLine(result.ToString() + " = DUPLICATE");
                            }
                        }
                        
                    }
                }

            }
            
            //sum true items in table
            UInt64 total_sum = 0;
            for (int i = 0; i < array_table.Length; i++)
            {

                if (array_table[i] == true)
                    total_sum += (UInt64)i;
            }
            Console.WriteLine(total_sum.ToString());
            Console.ReadLine();

        }//end of main
    }
}