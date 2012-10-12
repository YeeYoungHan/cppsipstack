<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%
	Connection m_clsDbConn = null;
	
	try
	{
		Class.forName("com.mysql.jdbc.Driver");
		
		m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
	}
	catch( Exception e )
	{
		e.printStackTrace( );
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>SIP Server List</title>
</head>
<body>
<b>SIP Server List</b><br>

<table>
	<tr><td align="right">
		<a href="SipServerInsert.jsp">AddSipServer</a>
	</td></tr>
	<tr><td>

		<table border=1 cellspacing=0 cellpadding=5>
			<tr bgcolor="#ffcccc" >
				<td width="100" align="center"><b>Id</b></td>
				<td width="80" align="center"><b>IP</b></td>
				<td width="50" align="center"><b>Port</b></td>
				<td width="100" align="center"><b>Domain</b></td>
				<td width="100" align="center"><b>UserId</b></td>
				<td width="80" align="center"><b>Timeout</b></td>
				<td width="70" align="center">&nbsp;</td>
			</tr>
<%
	if( m_clsDbConn != null )
	{
		Statement clsStmt = m_clsDbConn.createStatement( );
		String strSQL = "SELECT Id, Ip, Port, Domain, UserId, LoginTimeout FROM sipserver ORDER BY Id";
		
		ResultSet clsRS = clsStmt.executeQuery( strSQL );
		String strBgColor;
		int	iRow = 0;
		
		while( clsRS.next() )
		{
			++iRow;
			if( iRow % 2 == 0 )
			{
				strBgColor = "bgcolor=\"#ffeeee\"";
			}
			else
			{
				strBgColor = "";
			}
%>
			<tr <%= strBgColor %>>
				<td><%= clsRS.getString( 1 ) %></td>
				<td><%= clsRS.getString( 2 ) %></td>
				<td align="center"><%= clsRS.getInt( 3 ) %></td>
				<td><%= clsRS.getString( 4 ) %></td>
				<td><%= clsRS.getString( 5 ) %></td>
				<td align="center"><%= clsRS.getInt( 6 ) %></td>
				<td><a href="SipServerUpdate.jsp?id=<%= clsRS.getString( 1 ) %>"><img src="img/update.png" border="0"></a></td>
			</tr>
<%			
		}
		
		clsRS.close( );
		clsStmt.close( );
	}
%>

		</table>
		
	</td></tr>
</table>		
</body>
</html>
<%
	try
	{
		if( m_clsDbConn != null ) m_clsDbConn.close();
	}
	catch( Exception e )
	{
		e.printStackTrace( );
	}
%>