//Euler35 - Circular Primes
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Euler35
{
    /// <summary>
    /// Prime Sieve Implementation
    /// </summary>
    public class Prime_Sieve<T>
    {
        /// <summary>
        /// Finds the list of primes below a given max
        /// </summary>
        /// <param name="max"></param>
        /// <returns></returns>
        public static IList<T> findPrimes(int max)
        {
            var vals = new List<T>(((int)(max / (Math.Log(max) - 1.08366))));
            var maxSquareRoot = Math.Sqrt(max);
            var eliminated = new System.Collections.BitArray(max + 1);

            object temp = 2;
            vals.Add((T)temp);

            for (int i = 3; i <= max; i += 2)
            {
                if (!eliminated[i])
                {
                    if (i < maxSquareRoot)
                    {
                        for (int j = i * i; j <= max; j += 2 * i)
                            eliminated[j] = true;
                    }
                    temp = new object();
                    temp = i;
                    vals.Add((T)temp);
                }
            }
            return vals;
        }
}
    class Program
    {
        static void Main(string[] args)
        {
            //create list of primes
            IList<int> prime_ints = Prime_Sieve<int>.findPrimes(1000000);
            List<int[]> prime_array = convert_int2array(prime_ints);

            int prime_count = 0;
            foreach (int[] p in prime_array)
            {
                //check each of the array's rotations to ensure it is 
                //  circular
                if (is_circular_prime(p, prime_ints))
                    prime_count++;
                Console.WriteLine(array2string(p) + " -> " + prime_count);
            }
            Console.WriteLine(prime_count.ToString());
            Console.ReadLine();
        }
        /// <summary>
        /// Converts an integer into an integer array
        /// </summary>
        /// <param name="primes"></param>
        /// <returns></returns>
        static public List<int[]> convert_int2array(IList<int> primes)
        {
            List<int[]> output = new List<int[]>();
            int[] res;
            String sarray;

            foreach (int prime in primes)
            {
                sarray = prime.ToString();
                res = new int[sarray.Length];
                for(int i=0; i<res.Length; i++)
                    res[i] = sarray[i] - '0';

                output.Add(res);
            }
            return output;
        }
        /// <summary>
        /// Converts an int array into a string for debugging
        /// </summary>
        /// <param name="array">int array to convert</param>
        /// <returns></returns>
        public static String array2string(int[] array)
        {
            String output = String.Empty;
            for (int i = 0; i < array.Length; i++)
                output += array[i].ToString();
            return output;
        }
        /// <summary>
        /// returns true if the integer array is in fact a 
        /// circular prime.
        /// </summary>
        /// <param name="val"></param>
        /// <param name="prime_list"></param>
        /// <returns></returns>
        public static Boolean is_circular_prime( int[] val, IList<int> prime_list)
        {
            //rotate through each prime configuration
            int[] temp1 = new int[val.Length];
            int[] temp2 = new int[val.Length];
            val.CopyTo(temp1, 0);
            val.CopyTo(temp2, 0);
            int rotated_value;
            
            for (int i = 0; i < val.Length; i++)
            {
                //rotate bits
                for (int j = 0; j < val.Length-1; j++)
                {
                    temp1[j] = temp2[j + 1];
                }
                temp1[val.Length - 1] = temp2[0];

                //convert array to integer
                rotated_value = array2int(temp1);
                if (!prime_list.Contains(rotated_value))
                    return false;

                //finish
                temp1.CopyTo(temp2,0);

            }// end of i for loop

            return true;
        }//end of (is_circular_prime)


        /// <summary>
        /// Converts an integer array back into an integer
        /// </summary>
        /// <param name="array">integer array to convert</param>
        /// <returns></returns>
        public static int array2int(int[] array)
        {
            int sum = 0;
            int len = array.Length - 1;

            for (int i = 0; i < array.Length; i++)
                sum += (int)(array[i] * Math.Pow(10, len--));

            return sum;
        }
    }
}
