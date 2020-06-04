package com.dexbox;

public class _pi
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: pi: ..");
		System.out.println("");

        // calculate pi
        double p = 0;
        double a;
        double b = 1.0e-3;
        int j = 1;
        int k = 1;
        do
        {    
            a = 1.0 / j;
            a = a * k;  
            p += a;  
            k = -k; 
            j += 2;    
            a = a > 0? a : -a;  

        } while(a > b);

        // trace
        System.out.println("pi = " + p * 4); 

        // trace
		System.out.println("");
		System.out.println("test: pi: ok");
        return 0;
    }
}

