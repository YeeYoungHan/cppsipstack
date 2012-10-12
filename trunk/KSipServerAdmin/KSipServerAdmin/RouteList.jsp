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
<title>RouteList</title>
</head>
<body>
<b>Route List</b>
<br>
<table>
	<tr><td align="right">
		<a href="RouteInsert.jsp">AddRoute</a>
	</td></tr>
	<tr><td>

	<table border=1 cellspacing=0 cellpadding=5>
		<tr bgcolor="#ffcccc" >
			<td width="100" align="center"><b>SipServer</b></td>
			<td width="100" align="center"><b>RoutePrefix</b></td>
			<td width="100" align="center"><b>DeletePrefix</b></td>
			<td width="70" align="center">&nbsp;</td>
		</tr>
<%
	if( m_clsDbConn != null )
	{
		Statement clsStmt = m_clsDbConn.createStatement( );
		String strSQL = "SELECT Id, Prefix, DeletePrefix FROM routeprefix ORDER BY Id";
		
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
			<td align="center"><%= clsRS.getString( 2 ) %></td>
			<td align="center"><%= clsRS.getString( 3 ) %></td>
			<td><a href="RouteUpdate.jsp?id=<%= clsRS.getString( 1 ) %>&prefix=<%= clsRS.getString( 2 ) %>"><img src="img/update.png" border=0></a></td>
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