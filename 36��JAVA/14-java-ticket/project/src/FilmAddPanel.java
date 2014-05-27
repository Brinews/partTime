import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

class FilmAddPanel extends JPanel
{
	private JButton mesg;
	private JPanel panel;
	private JPanel btnPanel;

	JTextField jtfName;
	JTextField jtfLength;
	JTextField jtfDesp;

	public FilmAddPanel(String index)
	{
		setLayout(new BorderLayout());

		mesg = new JButton("  ");
		mesg.setEnabled(false);
		add(mesg, BorderLayout.NORTH);

		DocumentListener listener = new KeyFieldListener();

		panel = new JPanel(new GridLayout(3, 2));
		addLabel("Film Name.......:");
		jtfName = new JTextField("", 30);
		panel.add(jtfName);
		jtfName.getDocument().addDocumentListener(listener);

		addLabel("Film Length.....:");
		jtfLength = new JTextField("", 8);
		panel.add(jtfLength);
		addLabel("Film Description:");
		jtfDesp = new JTextField("", 10);
		panel.add(jtfDesp);

		add(panel, BorderLayout.CENTER);

		ActionListener command = new MemCommandAction();

		btnPanel = new JPanel();
		addButton("[+] Add", command);
		addButton("[-] Del", command);
		addButton("[u] Upd", command);
		
		add(btnPanel, BorderLayout.SOUTH);
	}

	private String buildAddSql()
	{
		String sql = "insert into FILM values ('";

		sql += jtfName.getText();
		sql += "', '";
		sql += jtfLength.getText();
		sql += "', '";
		sql += jtfDesp.getText();

		sql += "')";

		return sql;
	}

	private String buildDelSql()
	{
		String sql = "delete from FILM where FilmName ='";
		sql += jtfName.getText();
		sql += "'";

		return sql;
	}

	private String buildUpdSql()
	{
		String sql = "update FILM set FilmLength = '";

		sql += jtfLength.getText();
		sql += "', FilmDesp = '";
		sql += jtfDesp.getText();

		sql += "' where FilmName = '";
		sql += jtfName.getText();
		sql += "'";

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

	public void updateField()
	{
		String filmName = jtfName.getText();
		if (!"".equals(filmName)) {
			MySqlProxy mySql = new MySqlProxy();

			String sql = "select * from Film where FilmName = '"+ filmName + "'";

			try {
				mySql.spyConnectDB();

				ResultSet rs = mySql.spyDoQuery(sql);
				while (rs.next()) {
					jtfLength.setText(rs.getString(2));
					jtfDesp.setText(rs.getString(3));
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

	private class KeyFieldListener implements DocumentListener
	{
		public void insertUpdate(DocumentEvent event) { updateField(); }
		public void removeUpdate(DocumentEvent event) { updateField(); }
		public void changedUpdate(DocumentEvent event) {}
	}

	private class MemCommandAction implements ActionListener
	{
		public void actionPerformed(ActionEvent event)
		{
			String input = event.getActionCommand();

			String sql = "";
			String ret = input + " >>>> ";

			if (input.equals("[+] Add")) sql = buildAddSql();
			if (input.equals("[-] Del")) sql = buildDelSql();
			if (input.equals("[u] Upd")) sql = buildUpdSql();

			MyLogProxy.logWrite(sql);

			MySqlProxy mySql = new MySqlProxy();

			try {

				mySql.spyConnectDB();
				ret += mySql.spyDoUpdate(sql);
				mySql.spyCloseConnection();

			} catch (Exception e) {
				MyLogProxy.logWrite(e.getMessage());
			}

			mesg.setText(ret);
		}
	}
}
