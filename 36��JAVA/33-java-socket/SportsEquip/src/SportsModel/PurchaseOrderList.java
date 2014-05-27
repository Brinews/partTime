package SportsModel;

public class PurchaseOrderList {
	PurchaseOrder head;
	int count;

	/**
	 * constuctor
	 */
	public PurchaseOrderList()
	{
		head = null;
		count = 0;
	}

	/**
	 * @return 	an array containing all the purchase orders in this
	 *  		list, if  this list is not empty, otherwise null
	 */
	public PurchaseOrder[] getPurchaseOrders()
	{
		PurchaseOrder[] list = new PurchaseOrder[count];
		PurchaseOrder po = head;

		int i = 0;
		while (i < count)
		{
			list[i] = po;
			po = po.next;

			i++;
		}

		return list;
	}

	/**
	 * add a purchase order
	 * @param po
	 */
	public void addPurchaseOrder(PurchaseOrder po)
	{
		if (null != po)
		{
			if (null == head) head = po;
			else
			{
				po.next = head;
				head = po;
			}

			count++;
		}
	}

	/**
	 * remove all purchase in list
	 * @return remove count
	 */
	public int clearPurchaseOrder()
	{
		int rcnt = 0;

		while (null != head)
		{
			PurchaseOrder po = head;
			head = head.next;
			
			po.next = null;
			po = null;
			rcnt++;
		}

        count = 0;
		head = null;

		return rcnt;
	}

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder();

		PurchaseOrder po = head;
		while (null != po)
		{
			strbld.append(po.toString());
			strbld.append("\t");

			po = po.next;
		}
        strbld.append("\n");

		return strbld.toString();
	}
}
