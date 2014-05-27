import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.sql.Connection;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Random;
import java.util.concurrent.CountDownLatch;

import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;
import oracle.jdbc.pool.OracleDataSource;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.ResultSet;

public class Master {

    static CountDownLatch doneSignal;
	
	public static ArrayList<PerfRecord> mixRes = new ArrayList<PerfRecord>();
	
	public static ArrayList<String> params = new ArrayList<String>();
	
	public static PerfRecord rec = new PerfRecord();

	static int threadno = 1;
	
	public static Integer finishedThread = 0;

	static long startTime;

	static long rampUp = 0;

	static int thinkTime = 0;

	static String sqlid = "";

	static String userName = "";
	
	static String password = "";

	static int count = 0;

	static int runtime = 10;
	
	static int threadnum = 0;
	
	static int concur = 0;
	
	static int mode = 1;

	static String runfile = "";

	static String dbname = "zgjs1";
	
	static String shrAcct = "";

	static OracleDataSource ds = null;
	
	public static boolean debug = false;
	
	public static boolean mix = false;
	
	public static Boolean pt = true;
	
	Random rand = new Random();
	
	public static void help() {
		System.out.println();
		System.out.println("=============================Command Line Parameters Help================================");
		System.out.println("### To specify the input options by command line ###");
		System.out.println("SYNOPSIS");
		System.out.println("	java -classpath $ORACLE_HOME/jdbc/lib/ojdbc15.jar Master ");
		System.out.println("	     -thinktime ThinkTime");
		System.out.println("	     -runtime runTime");
		System.out.println("	     -rampup rampUpTime");
		System.out.println("EXAMPLE");
		System.out.println("	java -classpath $ORACLE_HOME/jdbc/lib/ojdbc15.jar Master ");
		System.out.println("	      -thinktime  -runtime 10 -rampup 10");
		System.out.println();
		System.exit(0);
	}

	public static void parseArgs(String[] args) {

		try {
			for (int i = 0; i < args.length; i = i + 2) {
				if (args[i].equalsIgnoreCase("-thinktime"))
					thinkTime = Integer.parseInt(args[i + 1]);
				else if (args[i].equalsIgnoreCase("-runtime"))
					runtime = Integer.parseInt(args[i + 1]);
				else if (args[i].equalsIgnoreCase("-rampup"))
					rampUp = Integer.parseInt(args[i + 1]);
				else if (args[i].equalsIgnoreCase("-runfile"))
					runfile = args[i + 1];
				else if (args[i].equalsIgnoreCase("-dbname"))
					dbname = args[i + 1];
				else if (args[i].equalsIgnoreCase("-user"))
					userName = args[i + 1];
				else if (args[i].equalsIgnoreCase("-pw"))
					password = args[i + 1];
				else if (args[i].equalsIgnoreCase("-sqlid"))
					sqlid = args[i + 1];
				else if (args[i].equalsIgnoreCase("-count"))
					count = Integer.parseInt(args[i + 1]);
				else if (args[i].equalsIgnoreCase("-shracct"))
					shrAcct = args[i + 1];
				else if (args[i].equalsIgnoreCase("-debug"))
					debug = Boolean.parseBoolean(args[i + 1]);
				//else if (args[i].equalsIgnoreCase("-mix"))
				//	mix = Boolean.parseBoolean(args[i + 1]);
				else if (args[i].equalsIgnoreCase("-mode"))
					mode = Integer.parseInt(args[i + 1]);
				else if (args[i].equalsIgnoreCase("-concur"))
					concur = Integer.parseInt(args[i + 1]);
			}
		} catch (Exception e) {
			e.printStackTrace();
			help();
		}

	}
	
	public static void initParamFromFile() {
		try {
		    FileInputStream fstream = new FileInputStream(runfile);
		    DataInputStream in = new DataInputStream(fstream);
		    BufferedReader br = new BufferedReader(new InputStreamReader(in));
		    String strLine;
		    while ((strLine = br.readLine()) != null) {
				if (strLine.startsWith("#"))
					continue;
				
				String fshrAcct = strLine;
				params.add(fshrAcct);
					
		    }	
				
		    in.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private static void runSQL(String sql, int count,int lineno) {
		if (sql.equals("QueryKH")) {
			for (int i = 0; i < count; i++) {
				new Thread(new QueryKH(threadno++, startTime, ds,lineno)).start();
			}
		}
	}
	
	public static void runFile() {
	
		startTime = System.currentTimeMillis() + rampUp * 1000;

		// BufferedReader bin = new BufferedReader(new InputStreamReader(System.in));

		try {

			FileInputStream fstream = new FileInputStream(runfile);

			DataInputStream in = new DataInputStream(fstream);

			BufferedReader br = new BufferedReader(new InputStreamReader(in));

			String strLine;

			if(!mix)

				System.out.println("Begin to Run Each SQL Concurrently...");

			else

				System.out.println("Begin to Run Mixed SQL Concurrently...");



			System.out.println();

			System.out.println("User       SQL           Threads RunTime(s) Start    End      TPS     Response(ms) Err(%)  TotalRun");

			System.out.println("---------- ------------- ------- ---------- -------- -------- ------- ------------ ------  ------------");

			int lineno=0;

			while ((strLine = br.readLine()) != null) {

				if (strLine.startsWith("#"))

					continue;

				String[] splits = strLine.split(",");

				String user = splits[0];

				String sql = splits[1];

				int count = Integer.parseInt(splits[2]);

				if (!mix) {

					doneSignal = new CountDownLatch(count);

					startTime = System.currentTimeMillis() + rampUp * 1000;

					String start = new SimpleDateFormat("HH:mm:ss").format(new Date(startTime));

					System.out.print(String.format("%10s %13s %7s %10s %8s ",user,sql,count,runtime,start));

					runSQL(sql, count,lineno++);

					doneSignal.await();

				}else{

					String start = new SimpleDateFormat("HH:mm:ss").format(new Date(startTime));

					PerfRecord record = new PerfRecord();

					record.ret = String.format("%10s %13s %7s %10s %8s ",user,sql,count,runtime,start);

					System.out.println(record.ret);

					synchronized(Master.mixRes){

					mixRes.add(record);

					}

					runSQL(sql, count,lineno++);

				}

			}



			if(mix){



				while(true){

					int finish;

					synchronized(Master.finishedThread){

						finish = Master.finishedThread.intValue();

						}

					if(finish == threadno-1)

						break;

					else

						Thread.sleep(1000);

				}



				System.out.println("User       SQL           Threads RunTime(s) Start    End      TPS     Response(ms) Err(%)");

				System.out.println("---------- ------------- ------- ---------- -------- -------- ------- ------------ ------");

				for(int i=0;i<mixRes.size();i++){

					PerfRecord record = mixRes.get(i);

					 String tps = new DecimalFormat("##.##").format(record.total*1.0/Master.runtime);

			 String resp = new DecimalFormat("##.##").format(record.elapsed/record.total);



					 String error = new DecimalFormat("##").format(record.errors / record.total*100);

					String ret = String.format("%8s %7s %12s %6s",record.end,tps,resp,error);

					System.out.println(mixRes.get(i).ret + ret);

				}

			}





			in.close();

		} catch (Exception e) {

			e.printStackTrace();

		}
/*		try {
			
			initParamFromFile();
			
			System.out.println("Begin to Run SQL Concurrently...");
			
			System.out.println();
			System.out.println("User       SQL           Threads RunTime(s) Start    End      TPS        Response(ms) Err(%)");
			System.out.println("---------- ------------- ------- ---------- -------- -------- ---------- ------------ ------");
			int lineno=0;
			
			doneSignal = new CountDownLatch(concur);
			startTime = System.currentTimeMillis() + rampUp * 1000;
			String start = new SimpleDateFormat("HH:mm:ss").format(new Date(startTime));
			System.out.println(String.format("%10s %13s %7s %10s %8s ",userName,"QueryKH",concur,runtime,start));
			runSQL("Query",concur,lineno++);
			doneSignal.await();		
					
			System.out.println("User       SQL           Threads RunTime(s) Start    End      TPS        Response(ms) Err(%) Totalrun     Elapsed(ms)");
			System.out.println("---------- ------------- ------- ---------- -------- -------- ---------- ------------ ------ ------------ ------------");
					
			synchronized (Master.rec) {
				String tps = new DecimalFormat("##.##").format(Master.rec.total*1.0/Master.runtime);
				String resp = new DecimalFormat("##.##").format(Master.rec.elapsed/Master.rec.total);
				String error = new DecimalFormat("##").format(Master.rec.errors / Master.rec.total*100);
				String totalrun = String.valueOf(Master.rec.total);
				String elapsed = String.valueOf(Master.rec.elapsed);
				System.out.println(String.format("%10s %13s %7s %10s %8s %8s %10s %12s %6s %12s %12s",userName,"QueryKH",concur,runtime,start,Master.rec.end,tps,resp,error,totalrun,elapsed));
			}
					
			
		} catch (Exception e) {
			e.printStackTrace();
		}
*/
	}
	
	public static void main(String[] args) throws Exception {
		
		parseArgs(args);
		
		System.setProperty("oracle.net.tns_admin", "/u01/app/oracle/product/11.2.0/dbhome_1/network/admin");
		ds = new OracleDataSource();
		ds.setURL("jdbc:oracle:thin:@" + dbname);
		ds.setUser(userName);
		ds.setPassword(password);
		
		if (!runfile.equals("")) {
			runFile();
		}
		else {
			System.out.println("User       SQL           Param      Threads RunTime(s) Start    End      TPS      Response(ms) Err(%)");
			System.out.println("---------- ------------- ---------- ------- ---------- -------- -------- -------- ------------ ------");
			
			params.add(shrAcct);
			startTime = System.currentTimeMillis() + rampUp * 1000;
			String start = new SimpleDateFormat("HH:mm:ss").format(new Date(startTime));
			System.out.println(String.format("%10s %13s %10s %7s %10s %8s ",userName,sqlid,shrAcct,concur,runtime,start));
			runSQL(sqlid, concur,0);
		}

	}

}