package SportsModel;

public class PurchaseOrder {

	OrderDate poDate;
	String poID;
	int poDiscount;
	Product poProduct;
	int poQuantity;

	PurchaseOrder next;

	/**
	 * constuctor for purchase order
	 * @param date, id, discount, product, quantity
	 */
	public PurchaseOrder(OrderDate date, String id,
			Product product, int quantity, int discount)
	{
		poDate = date;
		poID = id;

		poProduct = product;
		poQuantity = quantity;

		// get from customerdetails
		poDiscount = discount;

		next = null;
	}

	public String getCustomerID()
	{
		return poID;
	}

	public double getFullPrice()
	{
		return poProduct.getProductPrice() * poQuantity;
	}

	public double getDiscountPrice()
	{
		return getFullPrice() * (100 - poDiscount) / 100;
	}

	/**
	 * tostring
	 */
    @Override
	public String toString()
	{
		StringBuilder strbld = new StringBuilder();
		
		strbld.append(poDate.toString());
		strbld.append(poID);
		strbld.append(poDiscount);
		strbld.append(poProduct.toString());
		strbld.append(poQuantity);

		return strbld.toString();
	}
}
