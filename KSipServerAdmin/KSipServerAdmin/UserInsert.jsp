<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%
	Connection m_clsDbConn = null;
	
	if( request.getMethod( ).equals( "POST" ) )
	{
		try
		{
			Class.forName("com.mysql.jdbc.Driver");
			
			m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
		}
		catch( Exception e )
		{
			e.printStackTrace( );
		}
		finally
		{
			try
			{
				if( m_clsDbConn != null ) m_clsDbConn.close();
			}
			catch( Exception e )
			{
				e.printStackTrace( );
			}
		}
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>UserInsert</title>
</head>
<body>
<b>Add SIP User</b>
<br><br>
<form name="user" method="post" action="UserInsert.jsp">
<table>
	<tr><td>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>UserId</b></td>
				<td width="400"><input type="text" name="id" ></td>
			</tr>
			<tr>
				<td><b>Password</b></td>
				<td><input type="password" name="password" ></td>
			</tr>
			<tr>
				<td><b>DND</b></td>
				<td>
					<input type="radio" name="dnd" value="Y"> Use Do Not Disturb 
					<input type="radio" name="dnd" value="N" checked> Not Use Do Not Disturb
				</td>
			</tr>
			<tr>
				<td><b>CallForward</b></td>
				<td><input type="text" name="callforward" ></td>
			</tr>		
		</table>
	</td></tr>
	<tr><td align="right">
		<input type="submit" value="SAVE">&nbsp;&nbsp;<input type="button" value="BACK" onClick="history.go(-1)">
	</td></tr>
</table>	
</form>
</body>
</html>