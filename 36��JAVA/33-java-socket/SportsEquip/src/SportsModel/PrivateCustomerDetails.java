package SportsModel;

public class PrivateCustomerDetails extends CustomerDetails
{
	Name customerName;

	public PrivateCustomerDetails(String pID, String[] pAddr, 
			String pCode, double pPrice, String[] name)
            throws IllegalCustomerIDException
	{
		super(pID, pAddr, pCode, pPrice);

		customerName = new Name(name);
	}

	/**
	 * implementation of getDiscount
	 * @retrun discount
	 */
    @Override
	public int getDiscount()
	{
		int discount;

		discount = (int)(15 * customerHistPrice / 500);

		if (discount > 15)
			discount = 15;

		return discount;
	}

	public String getCustomerName()
	{
		return customerName.getName();
	}

	public void setCustomerName(String []names)
	{
		customerName.setName(names);
	}

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(super.toString());

		strbld.append(customerName.toString());
		strbld.append(getDiscount());

		return strbld.toString();
	}
}
