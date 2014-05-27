package cn.chinaclear.accout;

import java.sql.*;

public class Account {

	/**
	 * @param args
	 * @author xclu
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		//jdbc args
		String jdbcURL = PropertiesHelper.getString("jdbcURL", Environment.getProperties(), ""); 
		String username = PropertiesHelper.getString("username", Environment.getProperties(), "");   
		String password = PropertiesHelper.getString("password", Environment.getProperties(), ""); 
		String driverName = PropertiesHelper.getString("driverName", Environment.getProperties(), ""); 
		
		//data 
		String INVSTR_Name="";
		String CERT_Type="";
		String CERT_NBR="";
		String SEC_ACCT="";
		String MKT_Type="";
		String ACCT_INDC="";
		Double IDNT_INFO_UNIQ_ID = new Double(0);
		String INVSTR_ID="";
		

		String SEC_ACCT_Type="";
		String SEC_ACCT_Use_Type="";
		String SEC_ACCT_STS="";
		Date Open_Date;
		Date CNL_Date;
		String CNTRY_CDE="";
		String Open_Area_CDE="";
		String Open_AGT_ORG_ID="";
		String Open_AGT_Net_ID="";
		String SEC_ACCT_Name="";
		String SEC_ACCT_ABBR="";
		String SEC_ACCT_ENG_Name="";
		String ORIG_CERT_NBR="";
		String CERT_VLD_Date="";
		String DOM_ABR_INDC="";
		String ORG_INDC="";
		String COMM_ADDR="";
		String PSTL_CDE="";
		String CNTCT_TEL="";
		String MOB_NBR="";
		String Fax="";
		String Email_ADDR="";
		String HomePG="";
		Date Birth_Date;
		String GND="";
		String OCCU_Type="";
		String EDU_LVL="";
		String ORG_CDE="";
		String ORG_CDE_VLD_Date="";
		String CORP_REP="";
		String CORP_Type="";
		String ENTRP_CHRC_Type="";
		String PUB_COMP_INDC="";
		String STAT_OWN_INDC="";
		String SEC_ACCT_FRZ_STS_Type="";
		String TA_SYS_INDC="";
		String Open_Fund_ACCT_ASGN_INDC="";
		String INVSTR_Type="";
		
		String ZH_ID="";
		
		
		
		
		
		
		//加载驱动程序以连接数据库  
		try {    
			Class.forName(driverName);   
			Connection conn = DriverManager.getConnection(jdbcURL, username, password);
			conn.setAutoCommit(false);
			PreparedStatement ps=conn.prepareStatement("select INVSTR_ID from KH_ACCOUNT where INVSTR_Name = ? and CERT_Type = ? and CERT_NBR = ?");
			ps.setString(1,INVSTR_Name);
			ps.setString(2,CERT_Type);
			ps.setString(3,CERT_NBR);
			ResultSet rs=ps.executeQuery();
			if(rs.absolute(1)){
				INVSTR_ID = rs.getString("INVSTR_ID");
				//search in ZH_ACCOUNT,but no result need for next steps 
				ps=conn.prepareStatement("select ZH_ID,INVSTR_ID from ZH_ACCOUNT where INVSTR_ID = ?");
				ps.setString(1,INVSTR_ID);
				ps.executeQuery();
				//
				ps=conn.prepareStatement("select SEC_ACCT,MKT_Type from INVSTR_ACCT_RELA where INVSTR_ID = ?");
				ps.setString(1,INVSTR_ID);
				ResultSet rs1=ps.executeQuery();
				if(rs.absolute(1)){
					//该客户具有交易权限，无需开通权限
					return ;
				}
				else{//需开通权限 insert values into table INVSTR_ACCT_RELA,JY_ACCOUNT
					ps=conn.prepareStatement("insert into INVSTR_ACCT_RELA(" +
							"MKT_Type CHAR(1),								"+
							"ACCT_INDC CHAR(1),								"+
							"SEC_ACCT CHAR(12),								"+
							"INVSTR_ID CHAR(13),							" +
							"INVSTR_Name VARCHAR(100),						" +
							"CERT_Type CHAR(2),								" +
							"CERT_NBR VARCHAR(40),							" +
							"IDNT_INFO_UNIQ_ID DECIMAL(12,0)				)" + 
							"values(										" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?)												");
					ps.setString(1,MKT_Type);
					ps.setString(2,ACCT_INDC);	
					ps.setString(3,SEC_ACCT);	
					ps.setString(4,INVSTR_ID);	
					ps.setString(5,INVSTR_Name);	
					ps.setString(6,CERT_Type);	
					ps.setString(7,CERT_NBR);	
					ps.setDouble(8,IDNT_INFO_UNIQ_ID);	
					ps.execute();
					
					ps=conn.prepareStatement("insert into JY_ACCOUNT(" +
							"SEC_ACCT CHAR(12) ,							" +
							"SEC_ACCT_Type CHAR(1) ,						" +
							"SEC_ACCT_Use_Type CHAR(1) ,					" +
							"SEC_ACCT_STS CHAR(1) ,							" +
							"MKT_Type CHAR(1) ,								" +
							"Open_Date DATE									" +
							"CNL_Date DATE 									" +
							"CNTRY_CDE CHAR(3) ,							" +
							"Open_Area_CDE VARCHAR(6) ,						" +
							"Open_AGT_ORG_ID CHAR(8) ,						" +
							"Open_AGT_Net_ID CHAR(8) ,						" +
							"SEC_ACCT_Name VARCHAR(100) ,					" +
							"SEC_ACCT_ABBR VARCHAR(100) ,					" +
							"SEC_ACCT_ENG_Name VARCHAR(100) ,				" +
							"CERT_Type CHAR(2) ,							" +
							"CERT_NBR VARCHAR(40) ,							" +
							"ORIG_CERT_NBR VARCHAR(40) ,					" +
							"CERT_VLD_Date VARCHAR(8) ,						" +
							"DOM_ABR_INDC CHAR(1) ,							" +
							"ORG_INDC CHAR(1) ,								" +
							"COMM_ADDR VARCHAR(160) ,						" +
							"PSTL_CDE VARCHAR(10) ,							" +
							"CNTCT_TEL VARCHAR(60) ,						" +
							"MOB_NBR VARCHAR(20) ,							" +
							"Fax VARCHAR(60) ,								" +
							"Email_ADDR VARCHAR(60) ,						" +
							"HomePG VARCHAR(60) ,							" +
							"Birth_Date DATE FORMAT							" +
							"GND CHAR(1) ,									" +
							"OCCU_Type CHAR(1) ,							" +
							"EDU_LVL CHAR(1) ,								" +
							"ORG_CDE VARCHAR(16) ,							" +
							"ORG_CDE_VLD_Date VARCHAR(8) ,					" +
							"CORP_REP VARCHAR(60) ,							" +
							"CORP_Type CHAR(1) ,							" +
							"ENTRP_CHRC_Type CHAR(1) ,						" +
							"PUB_COMP_INDC CHAR(1) ,						" +
							"STAT_OWN_INDC CHAR(1) ,						" +
							"SEC_ACCT_FRZ_STS_Type CHAR(1) ,				" +
							"TA_SYS_INDC CHAR(1) ,							" +
							"Open_Fund_ACCT_ASGN_INDC CHAR(1) ,				" +
							"INVSTR_Type CHAR(3) 							)"+
							"values(										" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +												
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							")												");
					ps.setString(1,SEC_ACCT);
					ps.setString(2,SEC_ACCT_Type);	
					ps.setString(3,SEC_ACCT_Use_Type);	//
					ps.setString(4,SEC_ACCT_STS);	
					ps.setString(5,MKT_Type);	
					//ps.setDate(6,Open_Date);	
					//ps.setDate(7,CNL_Date);	
					ps.setString(8,CNTRY_CDE);
					
					ps.setString(1,Open_Area_CDE);
					ps.setString(2,Open_AGT_ORG_ID);	
					ps.setString(3,Open_AGT_Net_ID);	//
					ps.setString(4,SEC_ACCT_Name);	
					ps.setString(5,SEC_ACCT_ABBR);	
					ps.setString(6,SEC_ACCT_ENG_Name);	
					ps.setString(7,CERT_Type);	
					ps.setString(8,CERT_NBR);
					
					ps.setString(1,ORIG_CERT_NBR);
					ps.setString(2,CERT_VLD_Date);	
					ps.setString(3,DOM_ABR_INDC);	//
					ps.setString(4,ORG_INDC);	
					ps.setString(5,COMM_ADDR);	
					ps.setString(6,PSTL_CDE);	
					ps.setString(7,CNTCT_TEL);	
					ps.setString(8,MOB_NBR);
					
					ps.setString(1,Fax);
					ps.setString(2,Email_ADDR);	
					ps.setString(3,HomePG);	//
					//ps.setDate(3,Birth_Date);	//
					ps.setString(4,GND);	
					ps.setString(5,EDU_LVL);	
					ps.setString(6,ORG_CDE);	
					ps.setString(7,ORG_CDE_VLD_Date);	
					ps.setString(8,CORP_REP);
					
					ps.setString(1,CORP_Type);
					ps.setString(2,ENTRP_CHRC_Type);	
					ps.setString(3,PUB_COMP_INDC);	//
					ps.setString(4,STAT_OWN_INDC);	
					ps.setString(5,SEC_ACCT_FRZ_STS_Type );	
					ps.setString(6,TA_SYS_INDC);	
					ps.setString(7,Open_Fund_ACCT_ASGN_INDC);	
					ps.setString(8,INVSTR_Type);

					
					ps.execute();
				}
			}
			else{//no INVSTR_ID,insert values into table KH_ACCOUNT,ZH_ACCOUNT,INVSTR_ACCT_RELA,JY_ACCOUNT
				
				//insert values into table KH_ACCOUNT
				ps=conn.prepareStatement("insert into KH_ACCOUNT(" +
						"INVSTR_ID CHAR(13) ,								" +
						"INVSTR_Name VARCHAR(100)  ,						" +
						"INVSTR_ABBR VARCHAR(100)  ,						" +
						"INVSTR_ENG_Name VARCHAR(100) ,						" +
						"CERT_Type CHAR(2)  ,								" +
						"CERT_NBR VARCHAR(40) 								" +
						"CERT_VLD_Date VARCHAR(8) 							" +
						"DOM_ABR_INDC CHAR(1) ,								" +
						"INVSTR_STS_Type CHAR(1) ,							" +
						"ORG_INDC CHAR(1) ,									" +
						"INVSTR_Type CHAR(3)  ,								" +
						"CNTRY_CDE CHAR(3)  ,								" +
						"Area_CDE VARCHAR(6)  ,								" +
						"COMM_ADDR VARCHAR(160) ,							" +
						"PSTL_CDE VARCHAR(10)  ,							" +
						"CNTCT_TEL VARCHAR(60)  ,							" +
						"MOB_NBR VARCHAR(20)  ,								" +
						"Fax VARCHAR(60) ,									" +
						"Email_ADDR VARCHAR(60) ,							" +
						"HomePG VARCHAR(60)  ,								" +
						"Birth_Date DATE ,									" +
						"GND CHAR(1) ,										" +
						"EDU_LVL CHAR(1)  ,									" +
						"OCCU_Type CHAR(1) ,								" +
						"ORG_CDE VARCHAR(16) ,								" +
						"ORG_CDE_VLD_Date VARCHAR(8) ,						" +
						"CORP_REP VARCHAR(60) ,								" +
						"CORP_Type CHAR(1)									" +
						"ENTRP_CHRC_Type CHAR(1)  ,							" +
						"PUB_COMP_INDC CHAR(1) ,							" +
						"STAT_OWN_INDC CHAR(1) ,							" +
						"values(										" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +												
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						")												");
				/*ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);*/
				
				ps.execute();
				
				
				
				
				//insert values into table ZH_ACCOUNT
				ps=conn.prepareStatement("insert into ZH_ACCOUNT(" +
						"ZH_ID CHAR(13) ,								" +
						"INVSTR_ID CHAR(13) ,							" +
						"values(										" +
						"?												" +
						"?												" +					
						")												");
				ps.setString(1,ZH_ID);
				ps.setString(2,INVSTR_ID);	
				ps.execute();
				
				
				ps=conn.prepareStatement("insert into INVSTR_ACCT_RELA(" +
						"MKT_Type CHAR(1),								"+
						"ACCT_INDC CHAR(1),								"+
						"SEC_ACCT CHAR(12),								"+
						"INVSTR_ID CHAR(13),							" +
						"INVSTR_Name VARCHAR(100),						" +
						"CERT_Type CHAR(2),								" +
						"CERT_NBR VARCHAR(40),							" +
						"IDNT_INFO_UNIQ_ID DECIMAL(12,0)				)" + 
						"values(										" +
						"?,												" +
						"?,												" +
						"?,												" +
						"?,												" +
						"?,												" +
						"?,												" +
						"?,												" +
						"?)												");
				ps.setString(1,MKT_Type);
				ps.setString(2,ACCT_INDC);	
				ps.setString(3,SEC_ACCT);	
				ps.setString(4,INVSTR_ID);	
				ps.setString(5,INVSTR_Name);	
				ps.setString(6,CERT_Type);	
				ps.setString(7,CERT_NBR);	
				ps.setDouble(8,IDNT_INFO_UNIQ_ID);	
				ps.execute();
				
				ps=conn.prepareStatement("insert into JY_ACCOUNT(" +
						"SEC_ACCT CHAR(12) ,							" +
						"SEC_ACCT_Type CHAR(1) ,						" +
						"SEC_ACCT_Use_Type CHAR(1) ,					" +
						"SEC_ACCT_STS CHAR(1) ,							" +
						"MKT_Type CHAR(1) ,								" +
						"Open_Date DATE									" +
						"CNL_Date DATE 									" +
						"CNTRY_CDE CHAR(3) ,							" +
						"Open_Area_CDE VARCHAR(6) ,						" +
						"Open_AGT_ORG_ID CHAR(8) ,						" +
						"Open_AGT_Net_ID CHAR(8) ,						" +
						"SEC_ACCT_Name VARCHAR(100) ,					" +
						"SEC_ACCT_ABBR VARCHAR(100) ,					" +
						"SEC_ACCT_ENG_Name VARCHAR(100) ,				" +
						"CERT_Type CHAR(2) ,							" +
						"CERT_NBR VARCHAR(40) ,							" +
						"ORIG_CERT_NBR VARCHAR(40) ,					" +
						"CERT_VLD_Date VARCHAR(8) ,						" +
						"DOM_ABR_INDC CHAR(1) ,							" +
						"ORG_INDC CHAR(1) ,								" +
						"COMM_ADDR VARCHAR(160) ,						" +
						"PSTL_CDE VARCHAR(10) ,							" +
						"CNTCT_TEL VARCHAR(60) ,						" +
						"MOB_NBR VARCHAR(20) ,							" +
						"Fax VARCHAR(60) ,								" +
						"Email_ADDR VARCHAR(60) ,						" +
						"HomePG VARCHAR(60) ,							" +
						"Birth_Date DATE FORMAT							" +
						"GND CHAR(1) ,									" +
						"OCCU_Type CHAR(1) ,							" +
						"EDU_LVL CHAR(1) ,								" +
						"ORG_CDE VARCHAR(16) ,							" +
						"ORG_CDE_VLD_Date VARCHAR(8) ,					" +
						"CORP_REP VARCHAR(60) ,							" +
						"CORP_Type CHAR(1) ,							" +
						"ENTRP_CHRC_Type CHAR(1) ,						" +
						"PUB_COMP_INDC CHAR(1) ,						" +
						"STAT_OWN_INDC CHAR(1) ,						" +
						"SEC_ACCT_FRZ_STS_Type CHAR(1) ,				" +
						"TA_SYS_INDC CHAR(1) ,							" +
						"Open_Fund_ACCT_ASGN_INDC CHAR(1) ,				" +
						"INVSTR_Type CHAR(3) 							)"+
						"values(										" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +												
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						")												");
				ps.setString(1,SEC_ACCT);
				ps.setString(2,SEC_ACCT_Type);	
				ps.setString(3,SEC_ACCT_Use_Type);	//
				ps.setString(4,SEC_ACCT_STS);	
				ps.setString(5,MKT_Type);	
				//ps.setDate(6,Open_Date);	
				//ps.setDate(7,CNL_Date);	
				ps.setString(8,CNTRY_CDE);
				
				ps.setString(1,Open_Area_CDE);
				ps.setString(2,Open_AGT_ORG_ID);	
				ps.setString(3,Open_AGT_Net_ID);	//
				ps.setString(4,SEC_ACCT_Name);	
				ps.setString(5,SEC_ACCT_ABBR);	
				ps.setString(6,SEC_ACCT_ENG_Name);	
				ps.setString(7,CERT_Type);	
				ps.setString(8,CERT_NBR);
				
				ps.setString(1,ORIG_CERT_NBR);
				ps.setString(2,CERT_VLD_Date);	
				ps.setString(3,DOM_ABR_INDC);	//
				ps.setString(4,ORG_INDC);	
				ps.setString(5,COMM_ADDR);	
				ps.setString(6,PSTL_CDE);	
				ps.setString(7,CNTCT_TEL);	
				ps.setString(8,MOB_NBR);
				
				ps.setString(1,Fax);
				ps.setString(2,Email_ADDR);	
				ps.setString(3,HomePG);	//
				//ps.setDate(3,Birth_Date);	//
				ps.setString(4,GND);	
				ps.setString(5,EDU_LVL);	
				ps.setString(6,ORG_CDE);	
				ps.setString(7,ORG_CDE_VLD_Date);	
				ps.setString(8,CORP_REP);
				
				ps.setString(1,CORP_Type);
				ps.setString(2,ENTRP_CHRC_Type);	
				ps.setString(3,PUB_COMP_INDC);	//
				ps.setString(4,STAT_OWN_INDC);	
				ps.setString(5,SEC_ACCT_FRZ_STS_Type );	
				ps.setString(6,TA_SYS_INDC);	
				ps.setString(7,Open_Fund_ACCT_ASGN_INDC);	
				ps.setString(8,INVSTR_Type);

				
				ps.execute();
			}
			
			

			
			
			conn.commit();
		}   
		//捕获加载驱动程序异常  
		catch ( Exception ex ) {   
			System.out.println(ex); 
		}
	}
	
	void bg(){
		
		//jdbc args
		String jdbcURL = PropertiesHelper.getString("jdbcURL", Environment.getProperties(), ""); 
		String username = PropertiesHelper.getString("username", Environment.getProperties(), "");   
		String password = PropertiesHelper.getString("password", Environment.getProperties(), ""); 
		String driverName = PropertiesHelper.getString("driverName", Environment.getProperties(), ""); 
		
		
		
		//data 
		String INVSTR_Name="";
		String CERT_Type="";
		String CERT_NBR="";
		String MKT_Type="";
		String ACCT_INDC="";
		Long IDNT_INFO_UNIQ_ID = new Long(0);
		Long INVSTR_ID = new Long(0);
		
		//加载驱动程序以连接数据库  
		try {    
			Class.forName(driverName);   
			Connection conn = DriverManager.getConnection(jdbcURL, username, password);
			conn.setAutoCommit(false);
			PreparedStatement ps=conn.prepareStatement("select INVSTR_ID from KH_ACCOUNT where INVSTR_Name = ? and CERT_Type = ? and CERT_NBR = ?");
			ps.setString(1,INVSTR_Name);
			ps.setString(2,CERT_Type);
			ps.setString(3,CERT_NBR);
			ResultSet rs=ps.executeQuery();
			if(rs.absolute(1)){
				INVSTR_ID = rs.getLong("INVSTR_ID");
				//search in ZH_ACCOUNT,but no result need for next steps 
				ps=conn.prepareStatement("select ZH_ID,INVSTR_ID from ZH_ACCOUNT where INVSTR_ID = ?");
				ps.setLong(1,INVSTR_ID);
				ps.executeQuery();
				//
				ps=conn.prepareStatement("select SEC_ACCT,MKT_Type from INVSTR_ACCT_RELA where INVSTR_ID = ?");
				ps.setLong(1,INVSTR_ID);
				ResultSet rs1=ps.executeQuery();
				if(rs.absolute(1)){
					//该客户具有交易权限，无需开通权限
					return ;
				}
				else{//需开通权限 insert values into table INVSTR_ACCT_RELA,JY_ACCOUNT
					ps=conn.prepareStatement("insert into INVSTR_ACCT_RELA(" +
							"MKT_Type CHAR(1),								"+
							"ACCT_INDC CHAR(1),								"+
							"SEC_ACCT CHAR(12),								"+
							"INVSTR_ID CHAR(13),							" +
							"INVSTR_Name VARCHAR(100),						" +
							"CERT_Type CHAR(2),								" +
							"CERT_NBR VARCHAR(40),							" +
							"IDNT_INFO_UNIQ_ID DECIMAL(12,0)				)" + 
							"values(										" +
							"?,												" +
							"?,												" +
							"next SEC_ACCT sequence value,					" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?,												" +
							"?)												");
					ps.setString(1,MKT_Type);
					ps.setString(2,ACCT_INDC);	
					//ps.setLong(3,invstrId);	//
					ps.setLong(4,INVSTR_ID);	
					ps.setString(5,INVSTR_Name);	
					ps.setString(6,CERT_Type);	
					ps.setString(7,CERT_NBR);	
					ps.setLong(8,IDNT_INFO_UNIQ_ID);	
					ps.execute();
					
					ps=conn.prepareStatement("insert into JY_ACCOUNT(" +
							"SEC_ACCT CHAR(12) ,							" +
							"SEC_ACCT_Type CHAR(1) ,						" +
							"SEC_ACCT_Use_Type CHAR(1) ,					" +
							"SEC_ACCT_STS CHAR(1) ,							" +
							"MKT_Type CHAR(1) ,								" +
							"Open_Date DATE									" +
							"CNL_Date DATE 									" +
							"CNTRY_CDE CHAR(3) ,							" +
							"Open_Area_CDE VARCHAR(6) ,						" +
							"Open_AGT_ORG_ID CHAR(8) ,						" +
							"Open_AGT_Net_ID CHAR(8) ,						" +
							"SEC_ACCT_Name VARCHAR(100) ,					" +
							"SEC_ACCT_ABBR VARCHAR(100) ,					" +
							"SEC_ACCT_ENG_Name VARCHAR(100) ,				" +
							"CERT_Type CHAR(2) ,							" +
							"CERT_NBR VARCHAR(40) ,							" +
							"ORIG_CERT_NBR VARCHAR(40) ,					" +
							"CERT_VLD_Date VARCHAR(8) ,						" +
							"DOM_ABR_INDC CHAR(1) ,							" +
							"ORG_INDC CHAR(1) ,								" +
							"COMM_ADDR VARCHAR(160) ,						" +
							"PSTL_CDE VARCHAR(10) ,							" +
							"CNTCT_TEL VARCHAR(60) ,						" +
							"MOB_NBR VARCHAR(20) ,							" +
							"Fax VARCHAR(60) ,								" +
							"Email_ADDR VARCHAR(60) ,						" +
							"HomePG VARCHAR(60) ,							" +
							"Birth_Date DATE FORMAT							" +
							"GND CHAR(1) ,									" +
							"OCCU_Type CHAR(1) ,							" +
							"EDU_LVL CHAR(1) ,								" +
							"ORG_CDE VARCHAR(16) ,							" +
							"ORG_CDE_VLD_Date VARCHAR(8) ,					" +
							"CORP_REP VARCHAR(60) ,							" +
							"CORP_Type CHAR(1) ,							" +
							"ENTRP_CHRC_Type CHAR(1) ,						" +
							"PUB_COMP_INDC CHAR(1) ,						" +
							"STAT_OWN_INDC CHAR(1) ,						" +
							"SEC_ACCT_FRZ_STS_Type CHAR(1) ,				" +
							"TA_SYS_INDC CHAR(1) ,							" +
							"Open_Fund_ACCT_ASGN_INDC CHAR(1) ,				" +
							"INVSTR_Type CHAR(3) 							)"+
							"values(										" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +												
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							"?												" +
							")												");
					/*ps.setString(1,SEC_ACCT);
					ps.setString(2,SEC_ACCT_Type);	
					ps.setLong(3,SEC_ACCT_Use);	//
					ps.setLong(4,SEC_ACCT_STS);	
					ps.setString(5,MKT_Type);	
					ps.setString(6,Open_Date);	
					ps.setString(7,CNL_Date);	
					ps.setLong(8,CNTRY_CDE);
					
					ps.setString(1,Open_Area_CDE);
					ps.setString(2,Open_AGT_ORG_ID);	
					ps.setLong(3,Open_AGT_Net_ID);	//
					ps.setLong(4,SEC_ACCT_Name);	
					ps.setString(5,SEC_ACCT_ABBR);	
					ps.setString(6,SEC_ACCT_ENG);	
					ps.setString(7,CERT_Type);	
					ps.setLong(8,CERT_NBR);
					
					ps.setString(1,ORIG_CERT_NBR);
					ps.setString(2,CERT_VLD_Date);	
					ps.setLong(3,DOM_ABR_INDC);	//
					ps.setLong(4,ORG_INDC);	
					ps.setString(5,COMM_ADDR);	
					ps.setString(6,PSTL_CDE);	
					ps.setString(7,CNTCT_TEL);	
					ps.setLong(8,MOB_NBR);
					
					ps.setString(1,Fax);
					ps.setString(2,Email);	
					ps.setLong(3,HomePG);	//
					ps.setLong(4,GND);	
					ps.setString(5,EDU_LVL);	
					ps.setString(6,ORG_CDE);	
					ps.setString(7,ORG_CDE_VLD_Date);	
					ps.setLong(8,CORP_REP);
					
					ps.setString(1,CORP_Type);
					ps.setString(2,ENTRP_CHRC_Type);	
					ps.setLong(3,PUB_COMP_INDC);	//
					ps.setLong(4,STAT_OWN_INDC);	
					ps.setString(5,SEC_ACCT_FRZ_STS_Type );	
					ps.setString(6,TA_SYS_INDC);	
					ps.setString(7,Open_Fund_ACCT_ASGN_INDC);	
					ps.setLong(8,INVSTR_Type);*/

					
					ps.execute();
				}
			}
			else{//no INVSTR_ID,insert values into table KH_ACCOUNT,ZH_ACCOUNT,INVSTR_ACCT_RELA,JY_ACCOUNT
				ps=conn.prepareStatement("insert into KH_ACCOUNT(" +
						"INVSTR_ID CHAR(13) ,								" +
						"INVSTR_Name VARCHAR(100)  ,						" +
						"INVSTR_ABBR VARCHAR(100)  ,						" +
						"INVSTR_ENG_Name VARCHAR(100) ,						" +
						"CERT_Type CHAR(2)  ,								" +
						"CERT_NBR VARCHAR(40) 								" +
						"CERT_VLD_Date VARCHAR(8) 							" +
						"DOM_ABR_INDC CHAR(1) ,								" +
						"INVSTR_STS_Type CHAR(1) ,							" +
						"ORG_INDC CHAR(1) ,									" +
						"INVSTR_Type CHAR(3)  ,								" +
						"CNTRY_CDE CHAR(3)  ,								" +
						"Area_CDE VARCHAR(6)  ,								" +
						"COMM_ADDR VARCHAR(160) ,							" +
						"PSTL_CDE VARCHAR(10)  ,							" +
						"CNTCT_TEL VARCHAR(60)  ,							" +
						"MOB_NBR VARCHAR(20)  ,								" +
						"Fax VARCHAR(60) ,									" +
						"Email_ADDR VARCHAR(60) ,							" +
						"HomePG VARCHAR(60)  ,								" +
						"Birth_Date DATE ,									" +
						"GND CHAR(1) ,										" +
						"EDU_LVL CHAR(1)  ,									" +
						"OCCU_Type CHAR(1) ,								" +
						"ORG_CDE VARCHAR(16) ,								" +
						"ORG_CDE_VLD_Date VARCHAR(8) ,						" +
						"CORP_REP VARCHAR(60) ,								" +
						"CORP_Type CHAR(1)									" +
						"ENTRP_CHRC_Type CHAR(1)  ,							" +
						"PUB_COMP_INDC CHAR(1) ,							" +
						"STAT_OWN_INDC CHAR(1) ,							" +
						"values(										" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +												
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						"?												" +
						")												");
				/*ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);	
				ps.setLong(3,invstrId);	//
				ps.setLong(4,invstrId);	
				ps.setString(5,invName);	
				ps.setString(6,certType);	
				ps.setString(7,certNBR);	
				ps.setLong(8,idntInfoUniqId);
				ps.setString(1,mktType);
				ps.setString(2,acctIndc);*/
				
				ps.execute();
			}
			
			
			ps.execute();
			
			
			conn.commit();
		}   
		//捕获加载驱动程序异常  
		catch ( Exception ex ) {   
			System.out.println(ex); 
		}
	}
}

