package SingleLinkedLists;

public class SingleLinkedList {

	Node head;

	public SingleLinkedList() {
		head = null;
	}

	public boolean isEmpty() {
		return (head == null);
	}

	// Method for inserting an object in the first position of a list
	public void insertFirst(Object o) {
		// Pack o into a Node 
		Node n = new Node(o);

		// put n at the beginning of the list
		n.next = head;

		// and modify the head of the list
		head = n;
	}

	// Method for inserting an object in the last position of a list                                         
	public void insertLast(Object o) {
		// Pack o into a Node
		Node n = new Node(o);

		// If the list is empty, we simply have to change head
		if (head == null) {
			head = n;
			return;
		}
		// The list is not empty, so we have to traverse it, stopping at the last element
		Node crt = head; 
		while (crt.next != null) 
			crt = crt.next;
		crt.next = n;
	}

	// Method for printing the entire list (demonstrates traversal)
	public void print() {
		for (Node crt = head; crt != null; crt = crt.next) 
			System.out.println(crt.content);
	}

	// Method for removing the first element in the list; the object is returned
	// Note that this is O(1), since we have a sequence of primitive operations
	public Object removeFirst() throws EmptyListException {

		if (head == null) throw new EmptyListException();

		// Put a "finger" on the current head
		Node crt = head;

		// Move the head to the appropriate spot
		head = head.next;

		// Figure out what we need to return
		Object o = crt.content;

		// Clear out all pointer - the garbage collector will take care of this area in memory
		crt.content = null;
		crt.next = null;

		// And we're done
		return o;
	}


	// Method for removing the last element in the list; the object is returned
	// This is O(n) because we need to traverse the list
	public Object removeLast() throws EmptyListException {

		if (head == null) throw new EmptyListException();

		Object o;

		// If there is only one element, we need to modify the head of the list
		if (head.next == null) {
			o = head.content;
			head.content = null;
			head = null;
			return o;
		}

		// We need to go to the second-to-last element
		Node crt = head;
		while (crt.next.next != null)
			crt = crt.next;

		// Now get the content
		o = crt.next.content;

		// Remove all references that are not needed
		crt.next.content = null;
		crt.next = null;

		// And we're done
		return o;
	}

	// Method for finding an object in a list; returns true if the object is found
	public boolean find (Object o) {
		if (head == null) return false;

		// We need an index in the list
		for (Node crt = head; crt != null; crt = crt.next) {
			if (o.equals(crt.content)) return true;
		}
		return false;
	}

	//added by class's requirement
	public Object removeBefore(Object o) throws EmptyListException, NoSuchElementException
	{
		if (head == null) throw new EmptyListException();

		if (o.equals(head.content)) 
			throw new NoSuchElementException();

		Object ret = null;

		Node prev = head, cur = head.next, pprev = null;
		while (null != cur)
		{
			if (o.equals(cur.content)) 
			{
				ret = prev.content;

				//delete the first node
				if (prev == head)
					head = cur;
				else
				{
					//delete found node
					pprev.next = cur;
				}

				// return the delete Object
				return ret;
			}

			pprev = prev;
			prev = cur;
			cur = cur.next;
		}

		// no found
		throw new NoSuchElementException();
	}

	public Object removeAfter(Object o) throws EmptyListException, NoSuchElementException
	{
		if (head == null) throw new EmptyListException();

		Node prev = head, cur = head.next;
		if (null == cur) throw new NoSuchElementException();

		Object ret = null;

		while (null != cur)
		{
			if (o.equals(prev.content))
			{
				ret = cur.content;

				//delete found node
				prev.next = cur.next;

				return ret;
			}

			prev = cur;
			cur = cur.next;
		}

		//no found
		throw new NoSuchElementException();
	}
}
