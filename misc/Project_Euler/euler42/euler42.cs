//Project Euler Problem 42
//   Triangle Words
using System;
using System.Collections.Generic;
using System.IO;

namespace Euler42
{
    class Program
    {
        static void Main(string[] args)
        {
            String input_file = "words.txt";
            List<String> words = new List<String>();
            List<Int64> triangles = new List<Int64>();

            //Decide on Max Triangle number
            int triangle_max = 25;
            int triangle_val = triangle_max*(triangle_max+1)/2;

            //read CSV file and return list of words
            words = Read_CSV_File(input_file);

            //compute triangle numbers and return list of integers
            triangles = Compute_Triangle_List(triangle_max);

            //iterate through words, checking if they are triangle numbers
            Int64 word_sum = new Int64();
            Int64 current_max = 0;
            int count = 0;
            foreach (String str in words)
            {
                //convert string to sum of character positiongs
                word_sum = 0;
                foreach( Char i in str ){
                    word_sum += i - 'A' + 1;
                }

                //make sure you chose a high enough Triangle number max to
                // avoid have a value higher than the max triangle number
                current_max = Math.Max( word_sum, current_max);
                if (current_max > triangle_val)
                {
                    Console.WriteLine("ERROR: value exceeds max of triangle list");
                    Console.ReadLine();
                }
               
                //if the index is not -1, thus item is in list, then increment count
                if( triangles.IndexOf( word_sum ) != -1 )
                    count++;
            }
            Console.WriteLine("There are " + count + " triangle numbers");
            Console.ReadLine();

        }

        public static List<String> Read_CSV_File( String input )
        {
            //return list of items
            List<String> words = new List<String>();

            //create read object
            StreamReader sr = new StreamReader(input);

            string strline = "";
            string item;
            string[] _values = null;
            
            //While the stream is open
            while(!sr.EndOfStream)
            {
                //read string line
                strline = sr.ReadLine();

                //split by commas
                _values = strline.Split(',');
                //for each comma-deliminated item
                foreach (string str in _values)
                {
                    //remove quotes
                    item = str.Replace("\"", "");
                    item = item.ToUpper();
                    
                    words.Add(item);
                }
            }// end of while(!sr.EndOfStream)
            sr.Close();
            return words;
        }//end of Read_CSV_File

        /// <summary>
        /// Computes a list of triangle number up to the value
        /// provided as input
        /// </summary>
        /// <param name="max">Maximum number you wish to compute triangle number for</param>
        /// <returns></returns>
        public static List<Int64> Compute_Triangle_List(int max)
        {
            List<Int64> lst = new List<Int64>();

            for (int i = 0; i < max; i++)
                lst.Add((Int64)(0.5 * (double)(i + 1) * (i + 2)));

            return lst;
        }//end of Compute Triangle List
    }
}

    