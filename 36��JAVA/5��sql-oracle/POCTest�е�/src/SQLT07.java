import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.Random;

import oracle.jdbc.OracleTypes;
import oracle.jdbc.pool.OracleDataSource;
import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;

import java.sql.ResultSet;

public class SQLT07 extends GenericSQL {

	private static String sql= 
		"SELECT shr_acct, COUNT(*) FROM evt_h_stm_dtl_src WHERE shr_acct=? GROUP BY shr_acct ";
	
	public SQLT07(int threadNo,long starttime, OracleDataSource ds,int lineno){
	  	super(threadNo,starttime,ds,lineno);
	  	try {
				cstmt = (OraclePreparedStatement)conn.prepareStatement(sql);
				
			} catch (SQLException e) {
				e.printStackTrace();
			}
	}
	
	public long exec()  {
		try {
			long threadStartTime = System.currentTimeMillis();
			
			int total = Master.params.size();
			String shracct = Master.params.get(rand.nextInt(total));
			
			cstmt.setString(1, shracct);
						
			ResultSet rs = cstmt.executeQuery();		
			
			int ret=0;
			while (rs.next() && ret < 1 && Master.debug ) {
				ret++;
				System.out.println(rs.getString(1));	
				System.out.println(rs.getString(2));
			}			
			rs.close();
			
			long threadEndTime = System.currentTimeMillis();
						
		//	if(Master.debug)
		//		System.out.println("SQLT07: returned " + ret + " rows");
						
			return threadEndTime - threadStartTime;
		} catch (SQLException e) {			
			e.printStackTrace();
			return 0;
		}
	}
}
