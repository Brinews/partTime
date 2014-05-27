package SportsModel;

public class Product {
	String productCode;
	double productPrice;

	/**
	 * Judge the product code: format(aa/ddd)
	 * @param code
	 * @return true or false
	 */
	boolean isProductCodeValid(String code)
	{
		if (code.length() != 6) return false;

		String a = code.substring(0, 2);
		String d = code.substring(3, 6);

		if (code.charAt(2) != '/') return false;

		//all letter
		for (int i = 0; i < 2; i++)
		{
			char c = a.charAt(i);
			if (!((c >= 'a' && c <= 'z') || 
					(c >= 'A' && c <= 'Z'))) {
				return false;
			}
		}

		//all digit
		for (int i = 0; i < 3; i++)
		{
			char p = d.charAt(i);
			if (!(p >= '0' && p <= '9'))
				return false;
		}

		return true;
	}

	public Product(String code, double price) 
		throws InvalidProductCodeException
	{
		if (null == code || !isProductCodeValid(code))
			throw new InvalidProductCodeException();

		productCode = code;
		productPrice = price;
	}

	public String getProductCode()
	{
		return productCode;
	}

	public void setProductCode(String code)
		throws InvalidProductCodeException
	{
		if (null == code || !isProductCodeValid(code))
			throw new InvalidProductCodeException();

		productCode = code;
	}

	public double getProductPrice()
	{
		return productPrice;
	}

	public void setProductPrice(double price)
	{
		productPrice = price;
	}

    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder(productCode);

		strbld.append(" ");
		strbld.append(productPrice);

		return strbld.toString();
	}
}
