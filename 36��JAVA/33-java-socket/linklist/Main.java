package SingleLinkedLists;

public class Main{
	public static void main(String[] args)
	{
		SingleLinkedList sla = new SingleLinkedList();
		
		sla.insertFirst("Hello");
		sla.insertFirst("Hello1");
		sla.insertFirst("Hello2");
		sla.insertFirst("Hello3");
		
		sla.print();
		System.out.println("");

		try {
			//sla.removeAfter("Hello");
			//sla.removeAfter("Hello2");
			//sla.removeAfter("Hello2");
			sla.removeBefore("Hello");
			sla.removeBefore("Hello3");
		}
		catch (Exception e) {
			System.out.println(e.toString());
		}

		sla.print();
	}
}
