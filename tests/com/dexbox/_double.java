package com.dexbox;

public class _double 
{
    static public int main(String[] args) 
    {
        // trace
        System.out.println("test: ===============================================================");
        System.out.println("test: double: ..");
        System.out.println("");

        // test method
        test_method();

        // test compare
        test_compare();

        // test convert
        test_convert();

        // test calculate
        test_calculate();

        // test constant
        test_constant();

        // test store and load
        test_store_and_load();

        // test array
        test_array();

        // trace
        System.out.println("");
        System.out.println("test: double: ok");
        return 0;
    }

    private static void test_method()
    {
        System.out.println(add("1d + 2d = '", 1d, 2d, "'"));
    }

    private static String add(String header, double value1, double value2, String footer)
    {
        return header + add(value1, value2) + footer;
    }

    private static double add(double value1, double value2) 
    {
        return value1 + value2;
    }

    private static void test_compare()
    {
        double value1 = 0d;
        double value2 = 0d;
        if (value1 <= value2)
        {
            System.out.println("0d <= 0d");
        }
        if (value1 >= value2)
        {
            System.out.println("0d >= 0d");
        }

        value1 = 0d;
        value2 = 1d;
        if (value1 < value2) 
        {
            System.out.println("0d < 1d");
        }
        if (value1 > value2)
        {
            System.out.println("0d > 1d");
        }

        value1 = 1d;
        value2 = 0d;
        if (value1 < value2) 
        {
            System.out.println("1d < 0d");
        }
        if (value1 > value2)
        {
            System.out.println("1d > 0d");
        }
    }

	private static void test_convert()
    {
		double d = 1.5d;
		System.out.println("(int)1.5f = " + (int)d);
		System.out.println("(long)1.5f = " + (long)d);
		int i = 1;
		System.out.println("(double)1 = " + (double)i);
		long l = 1L;
		System.out.println("(double)1L = " + (double)l);
	}

	private static void test_calculate() 
    {
		double d1 = 4d, d2 = 2d;
		System.out.println("4.0 + 2.0 = " + (d1 + d2));
		System.out.println("4.0 - 2.0 = " + (d1 - d2));
		System.out.println("4.0 * 2.0 = " + (d1 * d2));
		System.out.println("4.0 / 2.0 = " + (d1 / d2));
		System.out.println("4.0 % 2.0 = " + (d1 % d2));
		System.out.println("-(4.0) = " + (-d1));
	}

	private static void test_store_and_load() 
    {
		double d0 = 0;
		System.out.println("d0 = " + d0);

		double d1 = 1;
		System.out.println("d1 = " + d1);

		double d2 = 2;
		System.out.println("d2 = " + d2);

		double d3 = 3;
		System.out.println("d3 = " + d3);

		double d4 = 4;
		System.out.println("d4 = " + d4);
	}

	private static void test_constant() 
    {
		System.out.println(0d);
		System.out.println(1d);
		System.out.println(2d);
		System.out.println(3d);
	}

    private static void test_array() 
    {
		double[] da = null;
		try 
        {
			System.out.println(da[0]);
			System.out.println("not reached1!");
		} 
        catch (NullPointerException e)
        {
		}

		try 
        {
			da[0] = 0d;
			System.out.println("not reached2!");
		}
        catch (NullPointerException e)
        {
		}

		da = new double[0];
		try 
        {
			System.out.println(da[1]);
			System.out.println("not reached3!");
		} 
        catch (ArrayIndexOutOfBoundsException e) 
        {
		}

		try
        {
			da[1] = 1d;
			System.out.println("not reached4!");
		} 
        catch (ArrayIndexOutOfBoundsException e) 
        {
		}

		da = new double[1];
		da[0] = 2d;
		System.out.println("da[0] is '" + da[0] + "'.");
	}
}

