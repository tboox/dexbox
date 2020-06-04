package com.dexbox;

public class _null 
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: null: ..");
		System.out.println("");

        // done
        String message = "hello world!";
        try
        {
            // check it
            assert message != null;
            message = null;
            assert message != null: "empty message!";
        }
        catch (AssertionError e)
        {
            // trace
            System.out.println(e.toString());
        }

        // print null
		System.out.println(message);

        // trace
		System.out.println("");
		System.out.println("test: null: ok");
        return 0;
    }
}

