package SingleLinkedLists;

public class EmptyListException extends Exception {

    public EmptyListException() {
		super("Illegal operation in an empty list");
    }
}

class NoSuchElementException extends Exception {
	
	public NoSuchElementException()
	{
		super ("No Such Element in the list");
	}
}
