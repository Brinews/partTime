package SportsModel;

class IDS {
		static String[] ids = {"PSC", "PWA", "PNI", "PNE", "PNW", "PMI", 
                    "PEA", "PSE", "PSW", "PGL", "CSC", "CWA", "CNI", "CNE", "CNW",
                    "CMI", "CEA", "CSE", "CSW", "CGL"};

		public IDS()
		{
		}

		public static int getIndex(String id)
		{
			if (id.length() != 8) return -1;

			String prefix = id.substring(0, 3);

			for (int i = 0; i < ids.length; i++)
				if (prefix.equals(ids[i])) return i;

			return -1;
		}

		public static int getLength()
		{
			return ids.length;
		}
}

public abstract class CustomerDetails
{
	String customerID;
	Address customerAddr;
	String customerRegCode;
	double customerHistPrice;

	CustomerDetails next;

	/**
	 * judge the ID is valid or not
	 * @param cuID
	 * @return true or false
	 */
	boolean isValidId(String cuID)
	{
		if (cuID.length() != 8) return false;

		//String prefix = cuID.substring(0, 3);

		if (cuID.substring(3, 4).equals("-"))
		{
			if (IDS.getIndex(cuID) != -1)
				return true;
		}

		return false;
	}

	/**
	 * constuctor for customer
	 * @param cuID, cuAddr, cuRegCode, cuHistPrice
	 * @throws IllegalCustomerIDException
	 */
	public CustomerDetails(String cuID, String[] cuAddr, String cuCode, double cuPrice)
            throws IllegalCustomerIDException
	{
		if (!isValidId(cuID))
			throw new IllegalCustomerIDException();

		customerID = cuID;
		customerAddr = new Address(cuAddr);
		customerRegCode = cuCode;
		customerHistPrice = cuPrice;

		next = null;
	}

	public String getCustomerID()
	{
		return customerID;
	}

	public String getCustomerAddress()
	{
		return customerAddr.getAddress();
	}

	public String getCustomerRegCode()
	{
		return customerRegCode;
	}

	public double getCustomerHistPrice()
	{
		return customerHistPrice;
	}

	public void setCustomerHistPrice(double price)
	{
		customerHistPrice = price;
	}

	public abstract int getDiscount();

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(customerID);

		strbld.append(customerAddr.toString());
		strbld.append(customerRegCode);
		strbld.append(customerHistPrice);

		return strbld.toString();
	}
}
