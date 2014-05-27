package SportsModel;

public class Name {

	String Title;
	String FirstName;
	String LastName;

	//constructor
	public Name(String[] names)
	{
		if (null == names || names.length != 3) return;

		Title = names[0];
		FirstName = names[1];
		LastName = names[2];
	}

	// basic set/get function
	public void setName(String[] names)
	{
		if (null == names || names.length != 3) return;

		Title = names[0];
		FirstName = names[1];
		LastName = names[2];
	}

	public String getName()
	{
		return Title + " " + FirstName + "," + LastName;
	}

	// required toString()
    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(getName());
		return strbld.toString();
	}
}
