package SportsModel;

public class OrderDate {

	String orderDate;
	int year, month, day;

	boolean ParseDate(String date)
	{
		if (8 != date.length()) return false;

		try {
			orderDate = date;

			day = Integer.parseInt(date.substring(0, 2));
			month = Integer.parseInt(date.substring(3, 5));
			year = Integer.parseInt(date.substring(6, 8));
		} 
		catch(Exception e)
		{
            System.out.println(e.toString());
			//e.printStackTrace();
		}

		return true;
	}

	/**
	 * Creates a new OrderDate
	 * @param date must have the format "dd/mm/yy" otherwise
	 * @throw IllegalDateFormatException
	 */
	public OrderDate(String date) throws IllegalDateFormatException
	{
		if (null == date ||
				false == ParseDate(date)) 
			throw new IllegalDateFormatException();

		if (getMonth() < 1 || getMonth() > 12)
			throw new IllegalDateFormatException();

		if (getDay() < 1 || getDay() > 31)
			throw new IllegalDateFormatException();
	}

	/**
	 * Date compare, 1='after', -1='before', 0='equal'
	 * @param anotherDate
	 * @return (-1, 0, 1)
	 */
	public int DateCompare(OrderDate anotherDate)
	{
		int ret = year - anotherDate.year;

		if (0 == ret)
			ret = month - anotherDate.month;

		if (0 == ret)
			ret = day - anotherDate.day;

		return ret;
	}

	public int getMonth()
	{
		return month;
	}
    
    public int getDay()
    {
        return day;
    }

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(orderDate);

		strbld.append(year);
		strbld.append(month);
		strbld.append(day);

		return strbld.toString();
	}
}
