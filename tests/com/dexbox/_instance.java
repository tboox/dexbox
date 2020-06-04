package com.dexbox;

public class _instance 
{
	static public int main(String[] args) 
    {
        // trace
		System.out.println("test: ===============================================================");
		System.out.println("test: instance: ..");
		System.out.println("");

        // done
        _instance instance = new _instance("hello world!");
		instance.public_method();
		instance.protected_method();
		instance.package_method();
		instance.private_method();
		System.out.println(instance.field);
		instance.field = "hello dexbox!";
		System.out.println(instance.field);

        // trace
		System.out.println("");
		System.out.println("test: instance: ok");
        return 0;
    }

	private String field = "hello ..!";

	private _instance() 
    {
	}
	private _instance(String field)
    {
		this.field = field;
	}
	public void public_method() 
    {
		System.out.println("public_method");
	}
	protected void protected_method() 
    {
		System.out.println("protected_method");
	}
	void package_method() 
    {
		System.out.println("package_method");
	}
	private void private_method() 
    {
		System.out.println("private_method");
	}
}

