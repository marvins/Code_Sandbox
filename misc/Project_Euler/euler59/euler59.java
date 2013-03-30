import java.io.FileReader;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.PrintWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.io.InputStreamReader;

public class hello {
    public static void main(String[] args)
            throws IOException {

        BufferedReader inputStream = null;
        BufferedWriter outputStream = null;

        try {
            inputStream  = new BufferedReader(new FileReader("cipher1.txt"));
            outputStream = new BufferedWriter(new FileWriter("output.txt"));

            String l;
            InputStreamReader reader = new InputStreamReader(System.in);
            BufferedReader in = new BufferedReader(reader);

            while ((l = inputStream.readLine()) != null) {

                //Convert string of numbers into list of integers
                List<String> prts = Arrays.asList(l.split(","));
                List<Integer> asciiList = new ArrayList<Integer>();

                //Build list of integers
                for( String str : prts ){
                    asciiList.add(Integer.parseInt(str));
                }
                 
                //iterate over possible code iterations
                char a = 'a';
                char z = 'z';
                char i,j,k;
                List<Integer> asciiOutput = new ArrayList<Integer>();
                for(i = a; i <= z; i++)
                    for(j = a; j <= z; j++)
                        for(k = a; k <= z; k++){
                            
                            //iterate over list
                            asciiOutput.clear();
                            for( int A = 0; A < asciiList.size(); A++ ){
                                if( A % 3 == 0 )
                                    asciiOutput.add(asciiList.get(A)^i);
                                else if( A % 3 == 1 )
                                    asciiOutput.add(asciiList.get(A)^j);
                                else
                                    asciiOutput.add(asciiList.get(A)^k);
                            }
                            outputStream.write(i);
                            outputStream.write(j);
                            outputStream.write(k);
                            outputStream.write(" : ");
                            Integer sum = 0;
                            for(Integer Z : asciiOutput){
                                outputStream.write(Character.toChars(Z));
                                sum += Z;
                            }
                            outputStream.write(" -> ");
                            outputStream.write(Integer.toString(sum));
                            outputStream.newLine();

                            System.out.print(i);
                            System.out.print(j);
                            System.out.print(k);
                            System.out.println();
                            //in.readLine();

                        }
                System.out.println("DONE");
            }
        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
    }
}