package com.dexbox;

public class _float 
{
    static public int main(String[] args) 
    {
        // trace
        System.out.println("test: ===============================================================");
        System.out.println("test: float: ..");
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
        System.out.println("test: float: ok");
        return 0;
    }

    private static void test_method()
    {
        System.out.println(add("1f + 2f = '", 1f, 2f, "'"));
    }

    private static String add(String header, float value1, float value2, String footer)
    {
        return header + add(value1, value2) + footer;
    }

    private static float add(float value1, float value2) 
    {
        return value1 + value2;
    }

    private static void test_compare()
    {
        float value1 = 0f;
        float value2 = 0f;
        if (value1 <= value2)
        {
            System.out.println("0f <= 0f");
        }
        if (value1 >= value2)
        {
            System.out.println("0f >= 0f");
        }

        value1 = 0f;
        value2 = 1f;
        if (value1 < value2) 
        {
            System.out.println("0f < 1f");
        }
        if (value1 > value2)
        {
            System.out.println("0f > 1f");
        }

        value1 = 1f;
        value2 = 0f;
        if (value1 < value2) 
        {
            System.out.println("1f < 0f");
        }
        if (value1 > value2)
        {
            System.out.println("1f > 0f");
        }
    }

	private static void test_convert()
    {
		float f = 1.5f;
		System.out.println("(int)1.5f = " + (int)f);
		System.out.println("(long)1.5f = " + (long)f);
		int i = 1;
		System.out.println("(float)1 = " + (float)i);
		long l = 1L;
		System.out.println("(float)1L = " + (float)l);
	}

	private static void test_calculate() 
    {
		float f1 = 4f, f2 = 2f;
		System.out.println("4.0 + 2.0 = " + (f1 + f2));
		System.out.println("4.0 - 2.0 = " + (f1 - f2));
		System.out.println("4.0 * 2.0 = " + (f1 * f2));
		System.out.println("4.0 / 2.0 = " + (f1 / f2));
		System.out.println("4.0 % 2.0 = " + (f1 % f2));
		System.out.println("-(4.0) = " + (-f1));
	}

	private static void test_store_and_load() 
    {
		float f0 = 0;
		System.out.println("f0 = " + f0);

		float f1 = 1;
		System.out.println("f1 = " + f1);

		float f2 = 2;
		System.out.println("f2 = " + f2);

		float f3 = 3;
		System.out.println("f3 = " + f3);

		float f4 = 4;
		System.out.println("f4 = " + f4);
	}

	private static void test_constant() 
    {
		System.out.println(0f);
		System.out.println(1f);
		System.out.println(2f);
		System.out.println(3f);
	}

    private static void test_array() 
    {
		float[] fa = null;
		try 
        {
			System.out.println(fa[0]);
			System.out.println("not reached1!");
		} 
        catch (NullPointerException e)
        {
		}

		try 
        {
			fa[0] = 0F;
			System.out.println("not reached2!");
		}
        catch (NullPointerException e)
        {
		}

		fa = new float[0];
		try 
        {
			System.out.println(fa[1]);
			System.out.println("not reached3!");
		} 
        catch (ArrayIndexOutOfBoundsException e) 
        {
		}

		try
        {
			fa[1] = 1F;
			System.out.println("not reached4!");
		} 
        catch (ArrayIndexOutOfBoundsException e) 
        {
		}

		fa = new float[1];
		fa[0] = 2f;
		System.out.println("fa[0] is '" + fa[0] + "'.");
	}
}

