import java.sql.Connection;import java.sql.PreparedStatement;
import java.sql.SQLException;
import data.Module1Set;
import oracle.jdbc.pool.OracleDataSource;


public class Module1 extends GenericSQL {
	
  /*SQL Text*/
  private static String sql=
  "SELECT * FROM (SELECT row_.*, rownum rn FROM (select eu.end_user_name, b.bank_name as END_USER_BANK_BANK_NAME, vc.id vc_id, vc.card_passwd vc_card_passwd, vc.vip_company_id vc_vip_company_id, vc.off_ratio vc_off_ratio, vc.status vc_status, vc.end_time vc_end_time, case when (vc.id is not null and vc.status = 0 and trunc(vc.end_time) >= trunc(sysdate)) then 1 else 0 end as isVip from user_data2.END_USER eu left join bank b on b.id = eu.END_USER_BANK_ID left join vip_card vc on eu.vip_card_id = vc.id where 1 = 1 and eu.END_USER_NAME like '%3a' and (eu.mc_site_id in (1) or eu.mc_site_id is null) order by eu.id ASC) row_ WHERE rownum <= 40)  WHERE RN > 30 " + "\n"; 
  protected PreparedStatement cstmt;
  
  /**/
  public Module1(int threadNo,long starttime, OracleDataSource ds,int lineno){
  	super(threadNo,starttime,ds,lineno);
  	try {
			cstmt = conn.prepareStatement(sql);
		} catch (SQLException e) {
			e.printStackTrace();
		}
  }
  
  public Module1(Connection conn){
  	try {
			cstmt = conn.prepareStatement(sql);
		} catch (SQLException e) {
			e.printStackTrace();
		}
  }
 
	public long exec()  {//Ö´ÐÐÊ±¼ä
//		int total = Master.AL_Module1set.size();
		try {
			//Module1Set set = Master.Module1set.get(rand.nextInt(total));
			//Module1Set set = Master.Module1set.get(1);
			/*Parameter registration*/
			//cstmt.setString(1, set.ID);

			/***********************/
			
			long threadStartTime = System.currentTimeMillis();
			cstmt.executeQuery();		
			long threadEndTime = System.currentTimeMillis();
			
			return threadEndTime - threadStartTime;
		} catch (SQLException e) {			
			e.printStackTrace();
			return 0;
		}
	}

}
