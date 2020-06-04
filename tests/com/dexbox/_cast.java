package com.dexbox;

public class _cast 
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: cast: ..");
		System.out.println("");
        
        int i = 0xFFFFFFFF;
		
		long l = (long)i;
		System.out.println("num = " + l);

		short s = (short)i;
		System.out.println("num = " + s);

		char c = (char)i;
		System.out.println("num = " + (int)c);
		
		byte b = (byte)i;
		System.out.println("num = " + b);

		l = 0xFFFFFFFFFFFFFFFFL;
		System.out.println("num = " + (int)l);

        // trace
		System.out.println("");
		System.out.println("test: cast: ok");
        return 0;
    }
}

