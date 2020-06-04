package com.dexbox;

public class _loop 
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: loop: ..");
		System.out.println("");

        // for loop
        int i = 0;
        int s = 0;
        for (i = 0; i < 10; i++)
        {
            s += i;
        }

        // check
		assert s == 45;

        // while loop
        s = 0;
        i = 10;
        while (i-- > 0)
        {
            s += i;
        }

        // check
		assert s == 45;

        // trace
		System.out.println("");
		System.out.println("test: loop: ok");
        return 0;
    }
}

