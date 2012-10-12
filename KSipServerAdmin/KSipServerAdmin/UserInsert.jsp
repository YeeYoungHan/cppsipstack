<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%
	Connection m_clsDbConn = null;
	String m_strId = "";
	String m_strPassWord = "";
	String m_strDnd = "";
	String m_strCallForward = "";
	String m_strErrorMsg = "";
	String m_strMeta = "";
	
	if( request.getMethod( ).equals( "POST" ) )
	{
		// 사용자 추가 기능
		try
		{
			m_strId = request.getParameter( "id" );
			m_strPassWord = request.getParameter( "password" );
			m_strDnd = request.getParameter( "dnd" );
			m_strCallForward = request.getParameter( "callforward" );
			
			if( m_strId.isEmpty( ) )
			{
				m_strErrorMsg = "please insert UserId.";
			}
			else if( m_strPassWord.isEmpty(  ) )
			{
				m_strErrorMsg = "please insert Password.";
			}
			else
			{
				Class.forName("com.mysql.jdbc.Driver");
				
				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "INSERT sipuser( Id, PassWord, DND, CallForward ) VALUES( ?, ?, ?, ? )";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strId );
				clsStmt.setString( 2, m_strPassWord );
				clsStmt.setString( 3, m_strDnd );
				clsStmt.setString( 4, m_strCallForward );
				
				clsStmt.executeUpdate( );
				
				m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=UserList.jsp\">";
			}			
		}
		catch( Exception e )
		{
			e.printStackTrace( );
			m_strErrorMsg = "DB Error - " + e.toString( );
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
<title>UserInsert</title>
</head>
<body>
<b>Add SIP User</b>
<br><br>
<font color=red><%= m_strErrorMsg %></font>
<form name="user" method="post" action="UserInsert.jsp">
<table>
	<tr><td>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>UserId</b></td>
				<td width="400"><input type="text" name="id" value="<%= m_strId %>" ></td>
			</tr>
			<tr>
				<td><b>Password</b></td>
				<td><input type="password" name="password" value="<%= m_strPassWord %>" ></td>
			</tr>
			<tr>
				<td><b>DND</b></td>
				<td>
					<input type="radio" name="dnd" value="Y" <%= strDndCheck %>> Use do not disturb 
					<input type="radio" name="dnd" value="N" <%= strNdndCheck %>> Not use do not disturb
				</td>
			</tr>
			<tr>
				<td><b>CallForward</b></td>
				<td><input type="text" name="callforward" value="<%= m_strCallForward %>" ></td>
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