<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%
	Connection m_clsDbConn = null;
	String m_strId = request.getParameter( "id" );
	String m_strPrefix = request.getParameter( "prefix" );
	String m_strDeletePrefix = "";
	String m_strErrorMsg = "";
	String m_strMeta = "";
	
	Class.forName("com.mysql.jdbc.Driver");
	
	if( m_strId.isEmpty(  ) )
	{
		m_strErrorMsg = "SipServerId is not found.";
	}
	else if( m_strPrefix.isEmpty( ) )
	{
		m_strErrorMsg = "Prefix is not found.";
	}
	else if( request.getMethod( ).equals( "POST" ) )
	{
		try
		{
			String strDelete = request.getParameter( "delete" );
			if( strDelete.equals( "Y" ) )
			{
				// Route 삭제 기능
				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "DELETE FROM routeprefix WHERE Id = ? AND Prefix = ?";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strId );
				clsStmt.setString( 2, m_strPrefix );
				
				clsStmt.executeUpdate( );
				
				m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=RouteList.jsp\">";
			}
			else
			{
				// Route 수정 기능
				m_strDeletePrefix = request.getParameter( "deleteprefix" );

				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "UPDATE routeprefix SET DeletePrefix = ? WHERE Id = ? AND Prefix = ?";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strDeletePrefix );
				clsStmt.setString( 2, m_strId );
				clsStmt.setString( 3, m_strPrefix );
					
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
	else
	{
		// Route 수정을 위한 Route 정보 보여주기 기능
		try
		{
			m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
			String strSQL = "SELECT DeletePrefix FROM routeprefix WHERE Id = ? AND Prefix = ?";
			PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
			clsStmt.setString( 1, m_strId );
			clsStmt.setString( 2, m_strPrefix );
			
			ResultSet clsRS = clsStmt.executeQuery( );
			if( clsRS.next(  ) )
			{
				m_strDeletePrefix = clsRS.getString( 1 );
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
<title>RouteUpdate</title>
<script>
	function checkDelete()
	{
		if( confirm( "Do you want to Delete?" ) )
		{
			document.route.delete.value = "Y";
			document.route.submit();
		}
	}
</script>
</head>
<body>
<b>Update Route</b>
<br><br>
<font color=red><%= m_strErrorMsg %></font>
<form name="route" method="post" action="RouteUpdate.jsp">
<input type="hidden" name="delete" value="N">
<table>
	<tr><td colspan=2>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>SipServerId</b></td>
				<td width="400"><input type="text" name="id" readonly="readonly" value="<%= m_strId %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>Prefix</b></td>
				<td width="400"><input type="text" name="prefix" readonly="readonly" value="<%= m_strPrefix %>" ></td>
			</tr>
			<tr>
				<td><b>DND</b></td>
				<td>
					<input type="radio" name="deleteprefix" value="Y" <%= strDeletePrefixCheck %>> Use delete prefix 
					<input type="radio" name="deleteprefix" value="N" <%= strNoDeletePrefixCheck %>> Not use delete prefix
				</td>
			</tr>
		</table>
	</td></tr>
	<tr>
		<td align="left">
			<input type="button" value="DELETE" onClick="checkDelete()">
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