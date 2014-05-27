package SportsModel;

public class CustomerDetailsList
{
	/**
	 * organized by array hash
	 */
	int customerTotal;
	CustomerDetails[]  heads = new CustomerDetails[IDS.getLength()];

	/**
	 * Constuctor for Customer Hash List
	 */
	public CustomerDetailsList()
	{
		for (int i = 0; i < heads.length; i++)
			heads[i] = null;

		customerTotal = 0;
	}

	public int getCustomerDetailsTotal()
	{
		return customerTotal;
	}

	/***
	 * Add a customer to customer list
	 * @param CustomerDetails, CustomerDetailsList
	 */
	public void addCustomer(CustomerDetails a)
	{
		String id = a.getCustomerID();
		int idx = IDS.getIndex(id);

		if (idx != -1) {
			a.next = heads[idx];
			heads[idx] = a;

			customerTotal ++;
		}
	}

	/**
	 * find a customer in customerlist
	 * @param givenID	the ID of a customer
	 * @return 			the customer's details if found, exception
	 *					thrown otherwise
	 */
	public CustomerDetails findCustomer(String givenID)
		throws CustomerNotFoundException
	{
		int idx = IDS.getIndex(givenID);

		if (idx == -1) 
			throw new CustomerNotFoundException();

		for (CustomerDetails ptr = heads[idx]; ptr != null; 
				ptr = ptr.next)
		{
			if (ptr.getCustomerID().equals(givenID)) {
				return ptr;
			}
		}

		throw new CustomerNotFoundException();
	}

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder();

		for (int i = 0; i < heads.length; i++)
		{
			CustomerDetails ptr = heads[i];

			while (null != ptr)
			{
				strbld.append(ptr.toString());
				strbld.append("\t");

				ptr = ptr.next;
			}
            strbld.append("\n");
		}

		return strbld.toString();
	}
}
