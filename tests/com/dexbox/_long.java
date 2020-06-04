package com.dexbox;

public class _long 
{
    static public int main(String[] args) 
    {
        // trace
        System.out.println("test: ===============================================================");
        System.out.println("test: long: ..");
        System.out.println("");

        // done
		long value = 8;
		System.out.println("val = " + (value + 2));
		System.out.println("val = " + (value - 2));
		System.out.println("val = " + (value * 2));
		System.out.println("val = " + (value / 2));
		System.out.println("val = " + (value % 2));
		System.out.println("val = " + (-value));
		System.out.println("val = " + (value << 2));
		System.out.println("val = " + (value >> 2));
		System.out.println("val = " + (value >>> 2));
		System.out.println("val = " + (value & 2));
		System.out.println("val = " + (value | 2));
		System.out.println("val = " + (value ^ 2));
		value++;
		System.out.println("val = " + value);

        // trace
        System.out.println("");
        System.out.println("test: long: ok");
        return 0;
    }
}

