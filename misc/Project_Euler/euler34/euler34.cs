using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Euler34
{
    class Program
    {
        //choose array size
        const int array_size = 7;

        static void Main(string[] args)
        {
            //build array
            int [] lst = new int[array_size];
            for (int i = 0; i < lst.Length; i++)
                lst[i] = 0;
            lst[lst.Length - 1] = 1;

            //print results
            UInt64 original, result, total = 0;
            for (int i = 1; i < 2540160; i++)
            {

                if (i > 3)
                {
                    original = array2uint(lst);
                    result = compute_index_factorial(ref lst);

                    if (original == result)
                    {
                        total += result;
                        Console.WriteLine("Match found at " + original);
                        Console.ReadLine();
                    }
                }
                increment_list(ref lst);
                
            }
            Console.WriteLine(total.ToString());
            Console.ReadLine();
        }

        /// <summary>
        /// Simple factorial function. Should use tail recursion.
        /// </summary>
        /// <param name="number">input value</param>
        /// <returns></returns>
        public static UInt64 Factorial( UInt64 number)
        {
            if (number <= 0)
                return 1;
            if (number == 1)
                return 1;
            if (number == 2)
                return 2;
            else
                return (number * Factorial(number - 1));
        }
        /// <summary>
        /// Converts an int array into a string, useful for
        /// debugging
        /// </summary>
        /// <param name="array">input number array for printing</param>
        /// <returns></returns>
        public static String array2string(ref int[] array)
        {
            //build string
            String str = "";
            for (int i = 0; i < array.Length; i++)
                str += (char)(array[i] + '0');

            return str;
        }

        /// <summary>
        /// Increments the value of the array
        /// </summary>
        /// <param name="array"></param>
        /// <returns></returns>
        public static void increment_list(ref int[] array)
        {
            for (int i = array.Length - 1; i >= 0; i--)
            {
                array[i] = array[i]+1;
                if (array[i] > 9)
                    array[i] = 0;
                else
                    break;
            }//end of for loop
            
        }//end of increment list
        /// <summary>
        /// Computes the sum of factorials of each item in the
        /// array.
        /// </summary>
        /// <param name="array"></param>
        /// <returns></returns>

        public static UInt64 compute_index_factorial(ref int[] array)
        {
            UInt64 sum = 0;
            bool set = false;
            for (int i = 0; i < array.Length; i++)
            {
                if (!set && array[i] != 0) set = true;
                if( set )
                    sum += Factorial((UInt64)array[i]);
            }
            //Console.WriteLine("INPUT: " + array2string(ref array));
            //Console.WriteLine(sum.ToString());
            //Console.ReadLine();
            return sum;
        }

        public static UInt64 array2uint(int[] array)
        {
            UInt64 sum = 0;
            int len = array.Length - 1;

            for(int i=0; i<array.Length; i++)
                sum += (UInt64)(array[i]*Math.Pow(10,len--));

            return sum;
        }
    }
}
