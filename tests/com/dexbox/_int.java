package com.dexbox;

public class _int 
{
    static public int main(String[] args) 
    {
        // trace
        System.out.println("test: ===============================================================");
        System.out.println("test: int: ..");
        System.out.println("");

        // done
		int value = 8;
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
        System.out.println("test: int: ok");
        return 0;
    }
}

