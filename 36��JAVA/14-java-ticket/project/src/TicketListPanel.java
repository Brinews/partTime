import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;
import java.util.*;

import java.util.Timer;
import java.util.TimerTask;
import javax.swing.SwingWorker;

class TicketListPanel extends JPanel
{
	private JButton mesg;
	private JPanel panel;
	private JPanel feature;
	private JPanel btnPanel;
	private JPanel infoPanel;

	private JButton btnBuy;
	private JButton btnReset;

	private Vector<String> seatID = new Vector<String>();
	private Vector<String> customerName = new Vector<String>();
	private Vector<String> seatSel = new Vector<String>();
	private Vector<String> seatPrice = new Vector<String>();
	private Vector<JCheckBox> jcbList = new Vector<JCheckBox>();

	JTextField jtfID;
	JTextField jtfPrice;
	JTextField jtfTicketSelled;
	JTextField jtfTicketEarning;

	private String showID = "0";
	private int row = 0, col = 0;
	private int buyed = 0, total = 0;
	private int priceTotal = 0;

	private static Timer time = new Timer("TICKET");

	public TicketListPanel()
	{
		setLayout(new BorderLayout());

		mesg = new JButton("  ");
		mesg.setEnabled(false);
		add(mesg, BorderLayout.NORTH);

		DocumentListener listener = new KeyFieldListener();

		JPanel center = new JPanel(new GridLayout(1, 2));

		panel = new JPanel(new GridLayout(4, 1));
		JPanel mPanel = new JPanel(new GridLayout(2, 2));
		mPanel.add(new JLabel("Show Moive Id......................:"));
		jtfID = new JTextField("", 10);
		mPanel.add(jtfID);
		mPanel.add(new JLabel("Each Seat Price....................:"));
		jtfPrice = new JTextField("", 5);
		mPanel.add(jtfPrice);
		jtfID.getDocument().addDocumentListener(listener);

		infoPanel = new JPanel(new GridLayout(2, 2));
		infoPanel.add(new JLabel("Seats Sell Out/Seats Total........:"));
		jtfTicketSelled = new JTextField("", 10);
		infoPanel.add(jtfTicketSelled);
		infoPanel.add(new JLabel("The Movie Show Earning............:"));
		jtfTicketEarning = new JTextField("", 10);
		infoPanel.add(jtfTicketEarning);

		infoPanel.setVisible(false);

		panel.add(mPanel);
		panel.add(infoPanel);
		panel.add(new JLabel(""));
		panel.add(new JLabel(""));
		
		feature = new JPanel();
		addFeatureCheckBox(0, 0);
		
		ActionListener command = new TicketCommandAction();
		btnPanel = new JPanel();
		btnBuy = addButton("Buy", command);
		btnReset = addButton("Refresh", command);

		center.add(panel);
		center.add(feature);
		//center.add(btnPanel);

		add(center, BorderLayout.CENTER);
		add(btnPanel, BorderLayout.SOUTH);

		time.schedule(new TimerTask() {
				@Override
				public void run() {update();}
				}, 0, 1000);
	}

	private void update()
	{
		new SwingWorker<Object, Object>()
		{
			protected Object doInBackground() throws Exception
			{
				infoDisplay();
				return null;
			}
		}.execute();
	}

	void infoDisplay()
	{
		if (UserInfo.userType.equals("0"))
			infoPanel.setVisible(true);
		else
			infoPanel.setVisible(false);
	}

	private String buildUpdSql()
	{
		String sql = "";

		getFeatureState();
		
		for (int i = 0; i < seatSel.size(); i++) {

			String seatId = seatSel.elementAt(i);

			sql += "update TicketInfo set TiktUserName = '";

			sql += UserInfo.userName;
			sql += "', TiktPrice = '"+ jtfPrice.getText() +"'";

			sql += " where TiktShowId = '";
			sql += jtfID.getText();
			sql += "' and TiktCinemaSeatId= '"+seatId+"';";
		}

		return sql;
	}

	private JButton addButton(String label, ActionListener listener)
	{
		JButton button = new JButton(label);
		button.addActionListener(listener);
		btnPanel.add(button);

		return button;
	}

	private void getFeatureState()
	{
		seatSel.clear();

		for (int i = 1; i <= jcbList.size(); i++)
		{
			JCheckBox j = jcbList.elementAt(i-1);

			if (j.isEnabled() && j.isSelected())
			{
				String seatId = "" + i;
				seatSel.add(seatId);
			}
		}
	}

	private void addFeatureCheckBox(int row, int col)
	{
		int number = 1;
		feature.removeAll();

		jcbList.clear();
		seatSel.clear();
		seatID.clear();
		customerName.clear();
		seatPrice.clear();

		JPanel grid = new JPanel(new GridLayout(row, 1));

		for (int i = 0; i < row; i++) {

			JPanel block = new JPanel();

			for (int j = 0; j < col; j++)
			{

				String name = "" + number;
				if (name.length() < 2) name = "0" + name;

				JCheckBox jcb = new JCheckBox(name);
				jcbList.add(jcb);
				jcb.setSelected(false);

				block.add(jcb);

				number++;
			}

			grid.add(block);
		}

		feature.add(grid);
	}

	private void setCheckBoxStatus(int row, int col)
	{
		int number = 0;

		buyed = 0;
		total = row*col;
		priceTotal = 0;

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col;j++)
			{
				JCheckBox jcb = jcbList.elementAt(number);
				String userName = customerName.elementAt(number);
				String sPrice = seatPrice.elementAt(number);

				if (!"-".equals(userName)) {
					jcb.setSelected(true);
					jcb.setEnabled(false);

					buyed++;
					priceTotal += Integer.parseInt(sPrice);
				}
				else {
					jcb.setSelected(false);
					jcb.setEnabled(true);
				}

				number++;
			}
		}

		jtfTicketSelled.setText("" + buyed + "/" + total);
		jtfTicketEarning.setText("" + priceTotal);
	}

	private void getFeatureInfo(String showId)
	{
		MySqlProxy mySql = new MySqlProxy();
		String sql = "select * from TicketInfo where TiktShowId = '" + showId + "' order by TiktCinemaSeatId";

		try {
			mySql.spyConnectDB();

			ResultSet rs = mySql.spyDoQuery(sql);
			while (rs.next()) {
				seatID.add(rs.getString(4));
				String userName = rs.getString(7);
				String sPrice = rs.getString(8);

				if (null == userName || "".equals(userName))
					userName = "-";
				customerName.add(userName);

				if (null == sPrice)
					sPrice = "0";
				seatPrice.add(sPrice);
			}

			rs.close();
			mySql.spyCloseConnection();
		}
		catch(Exception e)
		{
			MyLogProxy.logWrite(e.getMessage());
		}
	}

	private void updateField()
	{
		String cid = jtfID.getText();

		showID = "-1";

		if (!"".equals(cid)) {
			MySqlProxy mySql = new MySqlProxy();

			String sql = "select * from MovieShow where MShowId = '"+ cid + "'";

			try {
				mySql.spyConnectDB();

				ResultSet rs = mySql.spyDoQuery(sql);
				while (rs.next()) {
					showID = rs.getString(3);
				}

				rs.close();

				mySql.spyCloseConnection();
			}
			catch(Exception e)
			{
				MyLogProxy.logWrite(e.getMessage());
			}
		}

		if (!"-1".equals(showID))
		{
			if ("0".equals(showID)) {
				row = 4;
				col = 6;
			} else if ("1".equals(showID)) {
				row = 6;
				col = 5;
			}
		}
		else {
			row = col = 0;
			mesg.setText("No Movie Infomation Found.");
		}

		addFeatureCheckBox(row, col);

		if (0 != row && !"-1".equals(showID))
		{
			getFeatureInfo(cid);
			setCheckBoxStatus(row, col);
		}
	}

	private class KeyFieldListener implements DocumentListener
	{
		public void insertUpdate(DocumentEvent event) { updateField(); }
		public void removeUpdate(DocumentEvent event) { updateField(); }
		public void changedUpdate(DocumentEvent event) {}
	}

	private class TicketCommandAction implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			String input = event.getActionCommand();

			String sql = "";
			String ret = input + " >>>> ";

			if (input.equals("Buy")) 
			{
				/* update to database */
				sql = buildUpdSql();

				MyLogProxy.logWrite(sql);

				MySqlProxy mySql = new MySqlProxy();

				try {

					mySql.spyConnectDB();
					ret += mySql.spyDoUpdate(sql);
					mySql.spyCloseConnection();

				} catch (Exception e) {
					MyLogProxy.logWrite(e.getMessage());
				}
			}
			else if (input.equals("Refresh"))
			{
				/* reread */
				//setCheckBoxStatus(row, col);
				updateField();
			}

			mesg.setText(ret);
		}
	}
}
