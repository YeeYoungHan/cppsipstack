<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%
	Connection m_clsDbConn = null;
	String m_strId = "";
	String m_strPrefix = "";
	String m_strDeletePrefix = "";
	String m_strErrorMsg = "";
	String m_strMeta = "";
	
	Class.forName("com.mysql.jdbc.Driver");
		
	if( request.getMethod( ).equals( "POST" ) )
	{
		try
		{
			m_strId = request.getParameter( "id" );
			m_strPrefix = request.getParameter( "prefix" );
			m_strDeletePrefix = request.getParameter( "deleteprefix" );

			if( m_strId.isEmpty(  ) )
			{
				m_strErrorMsg = "SipServerId is not found.";
			}
			else if( m_strPrefix.isEmpty( ) )
			{
				m_strErrorMsg = "Prefix is not found.";
			}
			else
			{
				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "INSERT routeprefix( Id, Prefix, DeletePrefix ) VALUES ( ?, ?, ? )";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strId );
				clsStmt.setString( 2, m_strPrefix );
				clsStmt.setString( 3, m_strDeletePrefix );
					
				clsStmt.executeUpdate( );
				
				m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=RouteList.jsp\">";
			}
		}
		catch( Exception e )
		{
			e.printStackTrace( );
			m_strErrorMsg = "DB Error - " + e.toString( );
		}
	}
	
	try
	{
		if( m_clsDbConn != null ) m_clsDbConn.close();
	}
	catch( Exception e )
	{
		e.printStackTrace( );
	}
	
	String strDeletePrefixCheck = "", strNoDeletePrefixCheck = "";
	
	if( m_strDeletePrefix.equals( "Y" ) )
	{
		strDeletePrefixCheck = "checked";
	}
	else
	{
		strNoDeletePrefixCheck = "checked";
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<%= m_strMeta %>
<title>RouteInsert</title>
</head>
<body>
<b>Add Route</b>
<br><br>
<font color=red><%= m_strErrorMsg %></font>
<form name="route" method="post" action="RouteInsert.jsp">
<table>
	<tr><td colspan=2>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>SipServerId</b></td>
				<td width="400"><input type="text" name="id" value="<%= m_strId %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>Prefix</b></td>
				<td width="400"><input type="text" name="prefix" value="<%= m_strPrefix %>" ></td>
			</tr>
			<tr>
				<td><b>DeletePrefix</b></td>
				<td>
					<input type="radio" name="deleteprefix" value="Y" <%= strDeletePrefixCheck %>> Use delete prefix 
					<input type="radio" name="deleteprefix" value="N" <%= strNoDeletePrefixCheck %>> Not use delete prefix
				</td>
			</tr>
		</table>
	</td></tr>
	<tr>
		<td align="left">
		</td>
		<td align="right">
			<input type="submit" value="SAVE">&nbsp;&nbsp;<input type="button" value="BACK" onClick="history.go(-1)">
		</td>
	</tr>
</table>	
</form>
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