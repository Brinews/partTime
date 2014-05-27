package Heap;

public class HeapFullException extends Exception {
    
    public HeapFullException() {
		super("Heap is full");
    }
}
