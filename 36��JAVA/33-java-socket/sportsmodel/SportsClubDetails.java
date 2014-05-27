package SportsModel;

public class SportsClubDetails extends CustomerDetails
{
	int clubDiscount;
	String clubName;

	/**
	 * Constuctor for sports club
	 * @param +cName, +cDiscount
	 */
	public SportsClubDetails(String cID, String[] cAddr, String cCode,
            double cPrice, String cName, int cDiscount)
            throws IllegalCustomerIDException
	{
		super(cID, cAddr, cCode, cPrice);

		clubName = cName;
		clubDiscount = cDiscount;
	}

    @Override
	public int getDiscount()
	{
		return clubDiscount;
	}

	public void setDiscount(int discount)
	{
		clubDiscount = discount;
	}

	public String getClubName()
	{
		return clubName;
	}

	public void setClubName(String cName)
	{
		clubName = cName;
	}

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(super.toString());

		strbld.append(clubName);
		strbld.append(getDiscount());

		return strbld.toString();
	}
}
