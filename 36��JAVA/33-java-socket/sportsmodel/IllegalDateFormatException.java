package SportsModel;

public class IllegalDateFormatException extends Exception {
    
    public IllegalDateFormatException() {
		super("Date is illegal format");
    }
}

class IllegalCustomerIDException extends Exception {
	public IllegalCustomerIDException() {
		super("Customer ID is illegal format");
	}
}

class CustomerNotFoundException extends Exception {
	public CustomerNotFoundException() {
		super("Customer not Found in list");
	}
}

class InvalidProductCodeException extends Exception {
	public InvalidProductCodeException() {
		super("Invalid Product Code.");
	}
}

class IncorrectPurchaseOrderException extends Exception {
	public IncorrectPurchaseOrderException() {
		super("Incorrect Purchase Order.");
	}
}
