package com.dexbox;

public class _exception
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: exception: ..");
		System.out.println("");

        // test throw
        test_throw();

        // test nest
        test_nest();

        // trace
		System.out.println("");
		System.out.println("test: exception: ok");
        return 0;
    }

	private static void test_throw()
    {
        try
        {
            // trace
		    System.out.println("throw: ..");

            // throw
            throw new NullPointerException();
        }
        catch (NullPointerException e)
        {
		    System.out.println("throw: ok");
        }
        finally
        {
		    System.out.println("throw: finally");
        }
	}


	private static void test_nest()
    {
        try
        {
            try
            {
                // trace
                System.out.println("throw 1: ..");

                // throw
                throw new NullPointerException();
            }
            catch (NullPointerException e)
            {
                System.out.println("throw 1: ok");
            }
 
            // trace
            System.out.println("throw 2: ..");

            // throw
            throw new RuntimeException();
        }
        catch (RuntimeException e)
        {
            System.out.println("throw 2: ok");
        }
	}
}

