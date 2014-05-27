package SportsModel;

public class Address {
	String addrStreet;
	String addrCity;
	String addrCode;

	public Address(String[] addr)
	{
		if (addr.length < 3) return;

		addrStreet = addr[0];
		addrCity = addr[1];
		addrCode = addr[2];
	}

	public Address(String s, String c, String n)
	{
		addrStreet = s;
		addrCity = c;
		addrCode = n;
	}

	//basic set/get function
	public void setAddress(String s, String c, String n)
	{
		addrStreet = s;
		addrCity = c;
		addrCode = n;
	}

	public String getAddress()
	{
		return addrStreet + " " + addrCity + " " + addrCode;
	}

	//  toString()
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(getAddress());

		return strbld.toString();
	}
}
