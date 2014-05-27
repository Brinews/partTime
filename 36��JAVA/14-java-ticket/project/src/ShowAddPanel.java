import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.*;

import java.util.Date;
import java.text.SimpleDateFormat;

class ShowAddPanel extends JPanel
{
	private JButton mesg;
	private JPanel panel;
	private JPanel btnPanel;

	JTextField jtfID;
	JTextField jtfFilmName;
	JTextField jtfCinemaId;
	JTextField jtfShowDate;
	JTextField jtfShowTime;

	public ShowAddPanel(String index)
	{
		setLayout(new BorderLayout());

		mesg = new JButton("  ");
		mesg.setEnabled(false);
		add(mesg, BorderLayout.NORTH);

		DocumentListener listener = new KeyFieldListener();

		JPanel center = new JPanel();

		panel = new JPanel(new GridLayout(5, 2));
		addLabel("Show - ID(Auto Increment)..:");
		jtfID = new JTextField("", 10);
		panel.add(jtfID);
		jtfID.getDocument().addDocumentListener(listener);

		addLabel("Show - Film Name...........:");
		jtfFilmName = new JTextField("", 30);
		panel.add(jtfFilmName);

		addLabel("Show - Cinema ID(0 or 1)...:");
		jtfCinemaId = new JTextField("", 30);
		panel.add(jtfCinemaId);

		addLabel("Show - Date(yyyy-MM-dd)....:");
		jtfShowDate = new JTextField("", 30);
		panel.add(jtfShowDate);

		addLabel("Show - Time(HH:mm:ss)......:");
		jtfShowTime = new JTextField("", 4);
		panel.add(jtfShowTime);

		Date today = new Date();
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		String time = format.format(today);

		jtfShowDate.setText(time.substring(0, 10));
		jtfShowTime.setText(time.substring(11, 19));

		center.add(panel);

		add(center, BorderLayout.CENTER);

		ActionListener command = new ShowCommandAction();

		btnPanel = new JPanel();
		addButton("[+] Add", command);
		addButton("[-] Del", command);
		addButton("[u] Upd", command);
		
		add(btnPanel, BorderLayout.SOUTH);
	}

	private String buildAddSeatsSql()
	{
		String sql = "";

		int seats = 30;
		if (jtfCinemaId.getText().equals("0"))
		{
			seats = 24;
		}

		/* insert into TicketInfo */
		for (int i = 1; i <= seats; i++) {
			sql += "insert into TicketInfo values('";
			sql += jtfID.getText();
			sql += "', '";
			sql += jtfFilmName.getText();
			sql += "', '";
			sql += jtfCinemaId.getText();
			sql += "', '";
			sql += i;

			sql += "', '";
			sql += jtfShowDate.getText();
			sql += "', '";
			sql += jtfShowTime.getText();
			sql += "', '-', '0');";
			/***
			sql += "', to_date('";
			sql += jtfShowDate.getText();
			sql += "', 'yyyy-mm-dd hh24:mi:ss'), to_date('";
			sql += jtfShowTime.getText();
			sql += "', 'yyyy-mm-dd hh24:mi:ss'), '', '');";
			****/
		}

		return sql;
	}

	private String buildAddShowSql()
	{
		int maxNo = getMaxShowNo() + 1;
		jtfID.setText(""+maxNo);

		String sql = "insert into MovieShow values ('";

		sql += jtfID.getText();
		sql += "', '";
		sql += jtfFilmName.getText();
		sql += "', '";
		sql += jtfCinemaId.getText();

		sql += "', '";
		sql += jtfShowDate.getText();
		sql += "', '";
		sql += jtfShowTime.getText();
		sql += "');";
		/***
		sql += "', to_date('";
		sql += jtfShowDate.getText();
		sql += "', 'yyyy-mm-dd hh24:mi:ss'), to_date('";
		sql += jtfShowTime.getText();
		sql += "', 'yyyy-mm-dd hh24:mi:ss') );";
		***/

		return sql;
	}

	private String buildDelSql()
	{
		String sql = "delete from MovieShow where MShowId ='";
		sql += jtfID.getText();
		sql += "'; ";

		/* delete ticket info */
		sql += "delete from TicketInfo where TiktShowId = '";
		sql += jtfID.getText();
		sql += "';";

		return sql;
	}

	private String buildUpdSql()
	{
		/***
		 * No Update CinemaId
		 */
		String sql = "update MovieShow set MFilmName = '";

		sql += jtfFilmName.getText();
		sql += "', MCinemaId = '";
		sql += jtfCinemaId.getText();
		sql += "', MShowDate = '";
		sql += jtfShowDate.getText();
		sql += "', MShowTime = '";
		sql += jtfShowTime.getText();

		sql += "' where MShowId = '";
		sql += jtfID.getText();
		sql += "';";

		return sql;
	}

	private void addLabel(String txt)
	{
		JLabel label = new JLabel(txt);
		panel.add(label);
	}

	private void addButton(String label, ActionListener listener)
	{
		JButton button = new JButton(label);
		button.addActionListener(listener);
		btnPanel.add(button);
	}

	private void updateField()
	{
		String cid = jtfID.getText();
		if (!"".equals(cid)) {
			MySqlProxy mySql = new MySqlProxy();

			String sql = "select * from MovieShow where MShowId = '"+ cid + "'";

			try {
				mySql.spyConnectDB();

				ResultSet rs = mySql.spyDoQuery(sql);
				while (rs.next()) {
					jtfFilmName.setText(rs.getString(2));
					jtfCinemaId.setText(rs.getString(3));
					jtfShowDate.setText(rs.getString(4));
					jtfShowTime.setText(rs.getString(5));
				}

				rs.close();

				mySql.spyCloseConnection();
			}
			catch(Exception e)
			{
				MyLogProxy.logWrite(e.getMessage());
			}
		}
	}

	private int getMaxShowNo()
	{
		int no = 1;

		String sql = "Select max(MShowId) from MovieShow";
		MySqlProxy mySql = new MySqlProxy();

		try {
			mySql.spyConnectDB();

			ResultSet rs = mySql.spyDoQuery(sql);
			while (rs.next()) {
				no = Integer.parseInt(rs.getString(1));
			}

			rs.close();

			mySql.spyCloseConnection();
		}
		catch (Exception e)
		{
			MyLogProxy.logWrite(e.getMessage());
		}

		return no;
	}

	private class KeyFieldListener implements DocumentListener
	{
		public void insertUpdate(DocumentEvent event) { updateField(); }
		public void removeUpdate(DocumentEvent event) { updateField(); }
		public void changedUpdate(DocumentEvent event) {}
	}

	private class ShowCommandAction implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			String input = event.getActionCommand();

			String sql = "";
			String ret = input + " >>>> ";

			if (input.equals("[+] Add"))
			{
				sql = buildAddShowSql();
				String r = executeSql(sql);

				System.out.println("r = " + r);

				if ("succeed.".equals(r))
				{
					sql = buildAddSeatsSql();
					System.out.println("sql = " + sql);
					r = executeSql(sql);
				}
			}
			else {

				if (input.equals("[-] Del")) sql = buildDelSql();
				if (input.equals("[u] Upd")) sql = buildUpdSql();

				ret += executeSql(sql);
			}

			mesg.setText(ret);
		}
	}

	private String executeSql(String sql)
	{
		String ret = "";
		MyLogProxy.logWrite(sql);

		MySqlProxy mySql = new MySqlProxy();

		try {

			mySql.spyConnectDB();
			ret = mySql.spyDoUpdate(sql);
			mySql.spyCloseConnection();

		} catch (Exception e) {
			MyLogProxy.logWrite(e.getMessage());
		}

		return ret;
	}
}
