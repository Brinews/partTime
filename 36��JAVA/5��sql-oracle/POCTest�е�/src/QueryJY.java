import java.sql.Connection;
import java.sql.ResultSet;

import java.sql.SQLException;
import java.util.ArrayList;

import oracle.jdbc.pool.OracleDataSource;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;




public class QueryJY extends GenericSQL {
	
//	protected PreparedStatement cstmt_kh;
//	protected PreparedStatement cstmt_rela;
//	protected PreparedStatement cstmt_jy;
	public static ArrayList<String> params = new ArrayList<String>();
 
	

  /*SQL Text*/

  private static String sqlkh =

	  "select INVSTR_ID,INVSTR_Name,INVSTR_ABBR,INVSTR_ENG_Name,CERT_Type," +
	  
	  "CERT_NBR,CERT_VLD_Date,DOM_ABR_INDC,INVSTR_STS_Type,ORG_INDC,INVSTR_Type," +
	  
	  "CNTRY_CDE,Area_CDE,COMM_ADDR,PSTL_CDE,CNTCT_TEL,MOB_NBR,Fax,Email_ADDR," +
	  
	  "HomePG,Birth_Date,GND,EDU_LVL,OCCU_Type,ORG_CDE,ORG_CDE_VLD_Date,CORP_REP," +
	  
	  "CORP_Type,ENTRP_CHRC_Type,PUB_COMP_INDC,STAT_OWN_INDC from TABLE KH_ACCOUNT " +
	  
	  "where INVSTR_Name=? and CERT_Type=? and CERT_NBR=? " + "\n"; 
  
  private static String sqlrela = 
	  
	  "select select MKT_Type,SEC_ACCT from INVSTR_ACCT_RELA where INVSTR_ID = ?" + "/n";
  
  private static String sqljy = 
	  
	  "select SEC_ACCT,SEC_ACCT_Type,SEC_ACCT_Use_Type,SEC_ACCT_STS,MKT_Type," +
	  
	  "Open_Date,CNL_Date,CNTRY_CDE,Open_Area_CDE,Open_AGT_ORG_ID,Open_AGT_Net_ID," +
	  
	  "SEC_ACCT_Name,SEC_ACCT_ABBR,SEC_ACCT_ENG_Name,CERT_Type,CERT_NBR,ORIG_CERT_NBR," +
	  
	  "CERT_VLD_Date,DOM_ABR_INDC,ORG_INDC,COMM_ADDR,PSTL_CDE,CNTCT_TEL,MOB_NBR," +
	  
	  "Fax,Email_ADDR,HomePG,Birth_Date,GND,OCCU_Type,EDU_LVL,ORG_CDE,ORG_CDE_VLD_Date," +
	  
	  "CORP_REP,CORP_Type,ENTRP_CHRC_Type,PUB_COMP_INDC,STAT_OWN_INDC," +
	  
	  "SEC_ACCT_FRZ_STS_Type,TA_SYS_INDC ,Open_Fund_ACCT_ASGN_INDC,INVSTR_Type" +
	  
	  " from JY_ACCOUNT where MKT_Type = ?,SEC_ACCT = ?";

  /**/

  public QueryJY(int threadNo,long starttime, OracleDataSource ds,int lineno){

  	super(threadNo,starttime,ds,lineno);

  	try {

//			cstmt_kh = conn.prepareStatement(sqlkh);
//			cstmt_rela = conn.prepareStatement(sqlrela);
//			cstmt_jy = conn.prepareStatement(sqljy);
  			cstmt = conn.prepareStatement(sqlkh);

		} catch (SQLException e) {

			e.printStackTrace();

		}

  }

  public QueryJY(Connection conn){

  	try {

//  			cstmt_kh = conn.prepareStatement(sqlkh);
//  			cstmt_rela = conn.prepareStatement(sqlrela);
//  			cstmt_jy = conn.prepareStatement(sqljy);
  			cstmt = conn.prepareStatement(sqlkh);

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
			
			cstmt = conn.prepareStatement(sqlrela);
			
			int ret = 0;
			while (rs.next() && ret < 1 && Master.debug ) {
				ret ++ ;
				System.out.println(rs.getString(1));
				cstmt.setString(1,rs.getString(1));
				
				ResultSet rs_rela = cstmt.executeQuery();
				
				cstmt = conn.prepareStatement(sqljy);
				
				int ret_rela = 0;
				while (rs.next() && ret < 1 && Master.debug ){
					ret_rela ++ ;
					cstmt.setString(1,rs.getString(1));
					cstmt.setString(2,rs.getString(2));
					
					ResultSet rs_jy = cstmt.executeQuery();
					
					int ret_jy = 0;
					while (rs.next() && ret < 1 && Master.debug ){
						ret ++ ;
						System.out.println(rs.getString(1));
					}
					
				}
				
//				System.out.println(rs.getString(2));
				
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

