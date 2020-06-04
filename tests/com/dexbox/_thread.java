package com.dexbox;

public class _thread 
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: thread: ..");
		System.out.println("");

        try
        {
            // test case1
            test_case1();

            // test case2
            test_case2();

            // test case3
            test_case3();
        }
        catch (java.lang.InterruptedException e)
        {
            // trace
            System.out.println("catch: InterruptedException");
        }
        catch (java.lang.Exception e)
        {
            // trace
            System.out.println("catch: Exception");
        }

        // trace
		System.out.println("");
		System.out.println("test: thread: ok");
        return 0;
    }

    public static void test_case1() throws Exception 
    {
		java.lang.Thread thread1 = new java.lang.Thread(new Runnable() 
        {
            // done
            public void run()
            {
                int i = 0;
                while (i < 10)
                {
                    // trace
                    System.out.println("case1: run1: " + i);

                    // sleep
                    try
                    {
                        java.lang.Thread.sleep(1000);
                    }
                    catch (java.lang.InterruptedException e)
                    {
                        System.out.println("case1: run1: sleep interrupted");
                    }

                    // next
                    i++;
                }
            }
		});

		java.lang.Thread thread2 = new java.lang.Thread(new Runnable() 
        {
            // done
            public void run()
            {
                int i = 0;
                while (i < 10)
                {
                    // trace
                    System.out.println("case1: run2: " + i);

                    // sleep
                    try
                    {
                        java.lang.Thread.sleep(1000);
                    }
                    catch (java.lang.InterruptedException e)
                    {
                        System.out.println("case1: run2: sleep interrupted");
                    }

                    // next
                    i++;
                }
            }
		});

        // trace
        System.out.println("case1: start");

        // start 
		thread1.start();
		thread2.start();

        // trace
        System.out.println("case1: join1");

        // join 
		thread1.join();

        // trace
        System.out.println("case1: join2");

        // join
		thread2.join();

        // trace
		System.out.println("case1: exit");
	}

    public static void test_case2() throws Exception 
    {
		java.lang.Thread thread1 = new java.lang.Thread(new Runnable() 
        {
            // done
            public void run()
            {
                int i = 0;
                while (i < 10)
                {
                    // trace
                    System.out.println("case2: run1: " + i);

                    // yield
                    if ((i & 1) == 1) java.lang.Thread.yield();

                    // next
                    i++;
                }
            }
		});

		java.lang.Thread thread2 = new java.lang.Thread(new Runnable() 
        {
            // done
            public void run()
            {
                int i = 0;
                while (i < 10)
                {
                    // trace
                    System.out.println("case2: run2: " + i);

                    // yield
                    if ((i & 1) == 1) java.lang.Thread.yield();

                    // next
                    i++;
                }
            }
		});

        // trace
        System.out.println("case2: start");

        // start 
		thread1.start();
		thread2.start();

        // trace
        System.out.println("case2: join1");

        // join 
		thread1.join();

        // trace
        System.out.println("case2: join2");

        // join
		thread2.join();

        // trace
		System.out.println("case2: exit");
	}

    // the lock
    private static Object lock = new String();

    // the counter
    private static int counter = 0;

    public static void test_case3() throws Exception 
    {
		java.lang.Thread thread1 = new java.lang.Thread(new Runnable() 
        {
            // done
            public void run()
            {
                int i = 0;
                while (i < 10)
                {
                    // lock
                    synchronized (lock) 
                    {
                        // trace
                        System.out.println("case3: run1: " + counter);

                        // update counter
                        counter++;

                        // yield
                        java.lang.Thread.yield();
                    }

                    // next
                    i++;
                }
            }
		});

		java.lang.Thread thread2 = new java.lang.Thread(new Runnable() 
        {
            // done
            public void run()
            {
                int i = 0;
                while (i < 10)
                {
                    // lock
                    synchronized (lock) 
                    {
                        // trace
                        System.out.println("case3: run2: " + counter);

                        // update counter
                        counter++;

                        // yield
                        java.lang.Thread.yield();
                    }

                    // next
                    i++;
                }
            }
		});

        // trace
        System.out.println("case3: start");

        // start 
		thread1.start();
		thread2.start();

        // trace
        System.out.println("case3: join1");

        // join 
		thread1.join();

        // trace
        System.out.println("case3: join2");

        // join
		thread2.join();

        // trace
		System.out.println("case3: exit");
    }
}

