package Heap;

public class HeapNode {
    
    Object content;
    int key;

    public HeapNode (Object o, int priority) {
	content = o;
	key = priority;;
    }
}