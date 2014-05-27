/* File: queue.h       

  Header file for a very basic linked-list based dynamic Queue class.

*/

class queue
{
  public:

    queue(); // constructor - constructs a new empty queue.
    void enqueue( int item ); // enqueues <item>.
    int dequeue();  // dequeues the front item.
    int front();   // returns the front item without dequeuing it.
    bool empty();  // true iff the queue contains no items.
    int size();  // the current number of items in the queue.
    int remove(int item); // removes all occurrances of <item> 
      // from the queue, returning the number removed.
      // Other items on the queue retain their relative order.

  private:
    class node  // node type for the linked list 
    {
       public:
           node(int new_data, node * next_node ){
              data = new_data ;
              next = next_node ;
           }
           int data ;
           node * next ;
    };

    node * front_p ; // pointer to the (node containing the) next item 
              //  which will be dequeud, or NULL if the queue is empty.

    node * back_p ; // pointer to the (node containing the) last item 
              // which was enqueued, or NULL if the queue is empty.

    int current_size ; // current number of elements in the queue.
};
