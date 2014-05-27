import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import java.sql.SQLException;
import java.util.ArrayList;

import data.Module1Set;

import oracle.jdbc.pool.OracleDataSource;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;




public class QueryKH extends GenericSQL {
	
	protected PreparedStatement cstmt;
	public static ArrayList<String> params = new ArrayList<String>();
 
	

  /*SQL Text*/

  private static String sql=

	  "select INVSTR_ID,INVSTR_Name,INVSTR_ABBR,INVSTR_ENG_Name,CERT_Type," +
	  
	  "CERT_NBR,CERT_VLD_Date,DOM_ABR_INDC,INVSTR_STS_Type,ORG_INDC,INVSTR_Type," +
	  
	  "CNTRY_CDE,Area_CDE,COMM_ADDR,PSTL_CDE,CNTCT_TEL,MOB_NBR,Fax,Email_ADDR," +
	  
	  "HomePG,Birth_Date,GND,EDU_LVL,OCCU_Type,ORG_CDE,ORG_CDE_VLD_Date,CORP_REP," +
	  
	  "CORP_Type,ENTRP_CHRC_Type,PUB_COMP_INDC,STAT_OWN_INDC from TABLE KH_ACCOUNT " +
	  
	  "where INVSTR_Name=? and CERT_Type=? and CERT_NBR=? " + "\n"; 
 

  /**/

  public QueryKH(int threadNo,long starttime, OracleDataSource ds,int lineno){

  	super(threadNo,starttime,ds,lineno);

  	try {

			cstmt = conn.prepareStatement(sql);

		} catch (SQLException e) {

			e.printStackTrace();

		}

  }

  public QueryKH(Connection conn){

  	try {

			cstmt = conn.prepareStatement(sql);

		} catch (SQLException e) {

			e.printStackTrace();

		}

  }

  public static void initParamFromFile() {
		try {
		    FileInputStream fstream = new FileInputStream("test.txt");
		    DataInputStream in = new DataInputStream(fstream);
		    BufferedReader br = new BufferedReader(new InputStreamReader(in));
		    String strLine;
		    while ((strLine = br.readLine()) != null) {
				if (strLine.startsWith("#"))
					continue;
				
				String testDate = strLine;
				params.add(testDate);					
		    }					
		    in.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public long exec()  {//Ö´ÐÐÊ±¼ä
//		initParamFromFile()£»
		String invstr_name;
		String cert_type;
		String cert_nbr;
//		for(int i=0;i<params.size();i++)
//		{
//			ArrayList<String> querydata = new ArrayList<String>();
//			querydata = params.get(i).toString().split("	");;
//			
//		}
		try {
			invstr_name = "";
			cert_type = "";
			cert_nbr = "";

			cstmt.setString(1,invstr_name);

			cstmt.setString(2,cert_type);

			cstmt.setString(3,cert_nbr);

			long threadStartTime = System.currentTimeMillis();

			ResultSet rs = cstmt.executeQuery();		
			
			int ret=0;
			while (rs.next() && ret < 1 && Master.debug ) {
				ret++;
				System.out.println(rs.getString(1));	
				System.out.println(rs.getString(2));
			}			
			rs.close();
//			cstmt.close();

			long threadEndTime = System.currentTimeMillis();



//			if(Master.debug)

//			System.out.println("p_oder: p_error_info=" + cstmt.getString(10) + ",p_error_no=" + cstmt.getInt(11));

			//if(cstmt.getInt(11)!=0)

			//	errors++;



			return threadEndTime - threadStartTime;

		} catch (SQLException e) {

			e.printStackTrace();

			return -5;

		}

	}



}

