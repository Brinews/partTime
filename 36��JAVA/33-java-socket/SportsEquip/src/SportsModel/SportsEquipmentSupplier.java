package SportsModel;

public class SportsEquipmentSupplier {

	int nowYear;
	int nowMonth; /* from 0 to 11 */

	int pi;
	Product[] sesProduct;
	CustomerDetailsList sesCustomerList;
	PurchaseOrderList[] sesOrderList;
    
    public int getMonth()
    {
        return nowMonth + 1;
    }

	/***
	 * constructor
	 * init the product list, customer list, purchase list
	 */
	public SportsEquipmentSupplier(int year, int month, Product[] pdu)
	{
		nowYear = year;
		nowMonth = month - 1;

		pi = pdu.length;
		sesProduct = new Product[pdu.length];
		for (int i = 0; i <pdu.length; i++)
			sesProduct[i] = pdu[i];

		sesCustomerList = new CustomerDetailsList();
		sesOrderList = new PurchaseOrderList[12];
		for (int i = 0; i < 12; i++)
		{
			sesOrderList[i] = new PurchaseOrderList();
		}
	}

	/**
	 * add new customer
	 */
	public void addNewCustomer(String line)
	{
		if (null == line) return;

		String[] items = line.split("/");

		if (items[0].charAt(0) == 'P') {
			//private customer
			if (items.length != 7) return;

			String[] addr = new String[3];
			for (int i = 0; i < 3; i++)
				addr[i] = items[i+4];

			String[] names = new String[3];
			for (int i = 0; i < 3; i++)
				names[i] = items[i+1];

			try {
				PrivateCustomerDetails pcd = 
					new PrivateCustomerDetails(items[0], 
							addr, addr[2], 0.0, names);
				sesCustomerList.addCustomer(pcd);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		} 
		else if (items[0].charAt(0) == 'C') {
			//club customer
			if (items.length != 6) return;

			String[] addr = new String[3];
			for (int i = 0; i < 3; i++)
				addr[i] = items[i+2];

			try {
				int discount = Integer.parseInt(items[5]);

				SportsClubDetails scd = 
					new SportsClubDetails(items[0], addr, addr[2],
							0.0, items[1], discount);

				sesCustomerList.addCustomer(scd);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		}
	}

	/***
	 * Generates a new purchase order record for the current month
	 * and updates record of purchasing customer
	 * @param date			a String with format "dd/mm/yy"
	 * @param customerID	must be the ID of a customer in the company's
	 *						customer records
	 * @param p				must be in the company's current product range
	 * @param qty			the number of items required of the product
	 * @throws IncorrentPurchaseOrderException
	 */
	public void addNewPurchaseOrder
		(String datestr, String customerID, String productCode, int qty)
		throws IncorrectPurchaseOrderException
	{
		OrderDate oDate = null;
		int month = -1;
		//1. check date
		try {
			oDate = new OrderDate(datestr);
			month = oDate.getMonth();
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}

		//2. find customer
		CustomerDetails cd = null;
		try {
			cd = sesCustomerList.findCustomer(customerID);
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}

		//3. get product
		Product pd = null;
		
		for (int i = 0; i < pi; i++)
		{
			if (sesProduct[i].getProductCode().equals(productCode))
			{
				pd = sesProduct[i];
			}
		}

		//4. add to order list
		PurchaseOrder po = null;
		if (null != oDate && null != cd && null != pd) {
			int discount = cd.getDiscount();

			try {
				po = new PurchaseOrder(oDate,
						customerID, pd, qty, discount);

				sesOrderList[month - 1].addPurchaseOrder(po);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		}

		//5. update customer
		double histPrice = 0.0;
		if (null != po) {
			try {
				//sum all the orders by customerID
				for (int i = 0; i < sesOrderList.length; i++)
				{
                                      
					PurchaseOrder[] sList = 
						sesOrderList[i].getPurchaseOrders();
					for (int j = 0; j < sList.length; j++)
					{
						if (sList[j].getCustomerID().equals(customerID))
						{
							histPrice += sList[j].getDiscountPrice();
						}
					}
				}

                if (null != cd)
                    cd.setCustomerHistPrice(histPrice);
                
                if (customerID.charAt(0) == 'P' && null != cd){
                    System.out.println(customerID + " " + cd.getDiscount());
                }
			}
			catch(Exception e)
			{
				System.out.println(e.toString());
			}
		}
	}

	/**
	 * increments the index of the current month, 12(december) is followed
     * by 1(january). updates this supplier's records as appropriate.
	 */
	public void updateMonth()
	{
        int ret;
        
		nowMonth++;
		nowMonth %= 12;
        
        ret = sesOrderList[nowMonth].clearPurchaseOrder();
		//System.out.println("clear list count : " + ret);
	}

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder();

		strbld.append(nowYear);
		strbld.append(nowMonth+1);
		strbld.append("\n");

		for (int i = 0; i < pi; i++)
			strbld.append(sesProduct[i].toString());
		strbld.append("\n");

		strbld.append(sesCustomerList.toString());
		strbld.append("\n");

		for (int i = 0; i < sesOrderList.length; i++)
			strbld.append(sesOrderList[i].toString());
		strbld.append("\n");

		return strbld.toString();
	}
}
