import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

import java.util.List;
import java.util.ArrayList;

class UserInfo
{
	public static String userName = "";
	public static String userType = "";
	public static boolean userStatus = false; //true when logon

	public UserInfo()
	{
	}

	public static void Reset()
	{
		userName = "";
		userType = "";
		userStatus = false;
	}
}

class UserLogonPanel extends JPanel
{
	private JButton mesg;
	private JPanel panel;
	private JPanel btnPanel;

	private List<JButton> paraBtnList;

	JTextField jtfName;
	JTextField jtfPwd;
	JButton jtfCurUser;

	public UserLogonPanel()
	{
		paraBtnList = new ArrayList<JButton>();

		setLayout(new BorderLayout());

		mesg = new JButton("  ");
		mesg.setEnabled(false);
		add(mesg, BorderLayout.NORTH);

		panel = new JPanel(new GridLayout(3, 2));

		addLabel("User Name...................:");
		jtfName = new JTextField("", 30);
		panel.add(jtfName);

		addLabel("Usr Password................:");
		jtfPwd = new JTextField("", 8);
		panel.add(jtfPwd);

		addLabel("Current User................:");
		jtfCurUser = new JButton("");
		jtfCurUser.setEnabled(false);
		jtfCurUser.setText(UserInfo.userName);
		panel.add(jtfCurUser);

		add(panel, BorderLayout.CENTER);

		ActionListener command = new UsrCommandAction();

		btnPanel = new JPanel();
		addButton("Logon", command);
		addButton("Logout", command);
		
		add(btnPanel, BorderLayout.SOUTH);
	}

	public void setBtnList(List<JButton> btnList)
	{
		for(JButton p:btnList)
		{
			paraBtnList.add(p);
		}
	}

	public void hideClickButton(int s, int e)
	{
		int i = 1;
		for(JButton p:paraBtnList)
		{
			p.setVisible(true);
			if (i >= s && i <= e)
				p.setVisible(false);
			i++;
		}
	}

	private String doUserLogon()
	{
		String ret = "";
		String sql = "select * from Users where UserName = '";

		sql += jtfName.getText();
		sql += "' and UserPassword = '";
		sql += jtfPwd.getText();
		sql += "'";

		MySqlProxy mySql = new MySqlProxy();

		try {
			mySql.spyConnectDB();

			ResultSet rs = mySql.spyDoQuery(sql);
			while (rs.next()) {
				UserInfo.userName = rs.getString(1);
				UserInfo.userType = rs.getString(2);
				UserInfo.userStatus = true;

				ret = "Logon Success.";

				if (UserInfo.userType.equals("0"))
					hideClickButton(7, 9);
				else
					hideClickButton(2, 5);

			}

			rs.close();

			mySql.spyCloseConnection();
		}
		catch(Exception e)
		{
			MyLogProxy.logWrite(e.getMessage());
		}

		if (UserInfo.userStatus == false) ret = "Logon Failed.";
		jtfCurUser.setText(UserInfo.userName);

		return ret;
	}

	private String doUserLogout()
	{
		String ret = "Logout Success.";
		jtfCurUser.setText("");

		UserInfo.Reset();
		hideClickButton(2, 6);

		return ret;
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

	private class UsrCommandAction implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			String input = event.getActionCommand();

			String sql = "";
			String ret = input + " >>>> ";

			if (input.equals("Logon")) ret += doUserLogon();
			if (input.equals("Logout")) ret += doUserLogout();

			MyLogProxy.logWrite(ret);

			mesg.setText(ret);
		}
	}
}
