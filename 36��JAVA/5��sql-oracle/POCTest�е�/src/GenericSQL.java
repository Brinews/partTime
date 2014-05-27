import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Random;

import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;
import oracle.jdbc.pool.OracleDataSource;

public abstract class GenericSQL implements Runnable {

	private int noThread = 0;

	private long starttime;

	private OracleDataSource ds;

	protected int lineno;

	private long elapsed = 0;

	private long totalrun = 0;

	Random rand = new Random();

	protected Connection conn;

	protected PreparedStatement cstmt;
	
	protected long errors = 0;
	
	public GenericSQL(){
		
	}

	public GenericSQL(int threadNo, long starttime, OracleDataSource ds, int lineno) {
		noThread = threadNo;
		this.starttime = starttime;
		this.ds = ds;
		try {
			conn = ds.getConnection();
			//String local = "alter session set parallel_force_local=true";
			//Statement stmt = conn.createStatement();
			//stmt.executeUpdate(local);
			//stmt.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		this.lineno = lineno;
	}

	public abstract long exec();

	public void run() {

        try {
			java.util.Date now = new java.util.Date();
			

			long currenttime = System.currentTimeMillis();

			while (currenttime < starttime) {
				Thread.sleep(1);
				currenttime = System.currentTimeMillis();
			}

            if(Master.mode != 1 ){
      	        System.out.println(new SimpleDateFormat("HH:mm:ss").format(new Date()) + ": thread " + this.noThread + " begin to run...");
            }
			
            while (currenttime < (starttime + Master.runtime * 1000)) {				
       
				currenttime = System.currentTimeMillis();

				long ret = exec();

				elapsed = elapsed + ret;
				if (ret != 0)
					totalrun++;

				Thread.sleep(Master.thinkTime * 1000);

			}

			String end = new SimpleDateFormat("HH:mm:ss").format(new Date());
			
			synchronized (Master.rec) {
				Master.rec.elapsed = Master.rec.elapsed + elapsed;
				Master.rec.total = Master.rec.total + totalrun;
				Master.rec.end = end;
				Master.rec.errors = Master.rec.errors + errors;
			}
			
			if ((Master.mode !=1) && (Master.mode !=2)) {
				boolean ph = false;
				synchronized (Master.pt) {
					if (Master.pt){
						ph = true;
						Master.pt = false;
					}
				}
				if (ph) {
					System.out.println("User       SQL           Thread  RunTime(s) Start    End      TPS        Response(ms) Err(%)");
					System.out.println("---------- ------------- ------- ---------- -------- -------- ---------- ------------ ------");
				}
				String tps = new DecimalFormat("##.##").format(totalrun * 1.0 / Master.runtime);
				String resp = new DecimalFormat("##.##").format(elapsed / totalrun);
				String error = new DecimalFormat("##").format(errors / totalrun*100);
				String userName = ds.getUser();
				String start = new SimpleDateFormat("HH:mm:ss").format(new Date(starttime));
				System.out.println(String.format("%10s %13s %7s %10s %8s %8s %10s %12s %6s",userName,"SQLT07",noThread,Master.runtime,start,end,tps,resp,error));
			}
			
			Master.doneSignal.countDown();
			
			if(cstmt!=null)
				cstmt.close();
			conn.close();

			synchronized (Master.finishedThread) {
				Master.finishedThread++;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
