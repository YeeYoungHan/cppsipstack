<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%
	Connection m_clsDbConn = null;
	String m_strId = request.getParameter( "id" );
	String m_strPassWord = "";
	String m_strDnd = "";
	String m_strCallForward = "";
	String m_strErrorMsg = "";
	String m_strMeta = "";
	
	Class.forName("com.mysql.jdbc.Driver");
	
	if( m_strId.isEmpty(  ) )
	{
		m_strErrorMsg = "UserId is not found.";
	}
	else if( request.getMethod( ).equals( "POST" ) )
	{
		try
		{
			String strDelete = request.getParameter( "delete" );
			if( strDelete.equals( "Y" ) )
			{
				// 사용자 삭제 기능
				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "DELETE FROM sipuser WHERE Id = ?";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strId );
				
				clsStmt.executeUpdate( );
				
				m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=UserList.jsp\">";
			}
			else
			{
				// 사용자 수정 기능
				m_strPassWord = request.getParameter( "password" );
				m_strDnd = request.getParameter( "dnd" );
				m_strCallForward = request.getParameter( "callforward" );
				
				if( m_strPassWord.isEmpty(  ) )
				{
					m_strErrorMsg = "please insert Password.";
				}
				else
				{
					m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
					String strSQL = "UPDATE sipuser SET PassWord = ?, DND = ?, CallForward = ? WHERE Id = ?";
					PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
					clsStmt.setString( 1, m_strPassWord );
					clsStmt.setString( 2, m_strDnd );
					clsStmt.setString( 3, m_strCallForward );
					clsStmt.setString( 4, m_strId );
					
					clsStmt.executeUpdate( );
					
					m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=UserList.jsp\">";
				}
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
		// 사용자 수정을 위한 사용자 정보 보여주기 기능
		try
		{
			m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
			String strSQL = "SELECT PassWord, DND, CallForward FROM sipuser WHERE Id = ?";
			PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
			clsStmt.setString( 1, m_strId );
			
			ResultSet clsRS = clsStmt.executeQuery( );
			if( clsRS.next(  ) )
			{
				m_strPassWord = clsRS.getString( 1 );
				m_strDnd = clsRS.getString( 2 );
				m_strCallForward = clsRS.getString( 3 );
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
	
	String strDndCheck = "", strNdndCheck = "";
	
	if( m_strDnd.equals( "Y" ) )
	{
		strDndCheck = "checked";
	}
	else
	{
		strNdndCheck = "checked";
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<%= m_strMeta %>
<title>UserUpdate</title>
<script>
	function checkDelete()
	{
		if( confirm( "Do you want to Delete <%= m_strId %> ?" ) )
		{
			document.user.delete.value = "Y";
			document.user.submit();
		}
	}
</script>
</head>
<body>
<b>Update SIP User</b>
<br><br>
<font color=red><%= m_strErrorMsg %></font>
<form name="user" method="post" action="UserUpdate.jsp">
<input type="hidden" name="delete" value="N">
<table>
	<tr><td colspan=2>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>UserId</b></td>
				<td width="400"><input type="text" name="id" readonly="readonly" value="<%= m_strId %>" ></td>
			</tr>
			<tr>
				<td><b>Password</b></td>
				<td><input type="password" name="password" value="<%= m_strPassWord %>" ></td>
			</tr>
			<tr>
				<td><b>DND</b></td>
				<td>
					<input type="radio" name="dnd" value="Y" <%= strDndCheck %>> Use Do Not Disturb 
					<input type="radio" name="dnd" value="N" <%= strNdndCheck %>> Not Use Do Not Disturb
				</td>
			</tr>
			<tr>
				<td><b>CallForward</b></td>
				<td><input type="text" name="callforward" value="<%= m_strCallForward %>" ></td>
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