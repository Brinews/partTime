package SportsModel;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

class InputFileData {
	public InputFileData()
	{
	}

	public String GetAllData(String filename)
	{
		FileInputStream inputstream = null;

		InputStreamReader reader;
		BufferedReader br;

		try {
			inputstream = new FileInputStream(filename);

			reader = new InputStreamReader(inputstream);

			br = new BufferedReader(reader);

			return readAll(br);

		} catch (IOException ex2) {
			ex2.printStackTrace();
			return null;
		} finally {
			if (inputstream != null) {
				try {
					inputstream.close();
				} 
				catch (IOException ex) {
				}
			}
		}
	}

	String readAll(BufferedReader br) throws IOException 
	{
        StringBuilder sb = new StringBuilder();

        String line;
        while ((line = br.readLine()) != null) {
            sb.append(line).append("\r\n");
        }

        return sb.toString();
    }
}

public class SportsEquipmentDemo {
    
	public static void main(String[] args)
	{
		InputFileData ifd = new InputFileData();

		String content;
		Product[] pdu = null;
		//1. read product data
		content = ifd.GetAllData("ProductData.txt");
		String[] lines = content.split("\r\n");
		try {
			int count = Integer.parseInt(lines[0]);
			String[] pcode = lines[1].split("#");
			String[] pprice = lines[2].split("#");

			pdu = new Product[count];
			for (int i = 0; i < count; i++)
			{
				double price = Double.parseDouble(pprice[i]);
				pdu[i] = new Product(pcode[i], price);
			}
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}

		SportsEquipmentSupplier ses 
			= new SportsEquipmentSupplier(2013, 1, pdu);

		//2. read customer data
		content = ifd.GetAllData("CustomerData.txt");
		lines = content.split("\r\n");
		try {
			for (int i = 0; i < lines.length; i++)
				ses.addNewCustomer(lines[i]);
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}

		//3. read order data
		content = ifd.GetAllData("PurchaseOrderData.txt");
		lines = content.split("\r\n");
		try {
			for (int i = 0; i < lines.length; i++)
			{
				String str = lines[i];
				String[] items = str.split("#");

                if (items[3].charAt(items[3].length()-1) == '@')
                {
                    items[3] = items[3].substring(0, items[3].length()-1);
                }
                
                int qty = Integer.parseInt(items[3]);
                
                //update month
                int m = Integer.parseInt(items[0].substring(3,5));
                if (ses.getMonth() != m)
                    ses.updateMonth();
                
				ses.addNewPurchaseOrder(items[0], 
						items[1], items[2], qty);

				
			}
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
		}
	}
}
