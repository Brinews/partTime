package Heap;

public class Main {
	public static void main(String[] args)
	{
		Heap h = new Heap();

		try {
		h.insert("High", 12);
		h.insert("Low", 10);
		h.insert("Mid", 8);
		h.insert("Very Large", 19);
		h.insert("Very Small", 1);
		h.insert("Think", 7);
		h.insert("IBM", 90);
		h.insert("Microsoft", 60);
		h.insert("Cisco", 30);
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}

		h.printHeap();

		try {
			System.out.println(h.getMax());
		} 
		catch (Exception e)
		{
			System.out.println(e.toString());
		}
	}
}
