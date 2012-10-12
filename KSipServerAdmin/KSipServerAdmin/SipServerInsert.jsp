<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%@ include file="DbInfo.jsp" %>
<%!
	public int GetInt( String strInput, int iDefaultValue )
	{
		if( strInput == null ) return iDefaultValue;
		
		return Integer.parseInt( strInput );
	}
%>
<%
	Connection m_clsDbConn = null;
	String m_strId = "";
	String m_strIp = "";
	int m_iPort = 5060;
	String m_strDomain = "";
	String m_strUserId = "";
	String m_strPassWord = "";
	int m_iLoginTimeout = 3600;
	String m_strErrorMsg = "";
	String m_strMeta = "";
		
	Class.forName("com.mysql.jdbc.Driver");
	
	if( request.getMethod( ).equals( "POST" ) )
	{
		try
		{
			m_strId = request.getParameter( "id" );
			m_strIp = request.getParameter( "ip" );
			m_iPort = GetInt( request.getParameter( "port" ), 5060 );
			m_strDomain = request.getParameter( "domain" );
			m_strUserId = request.getParameter( "userid" );
			m_strPassWord = request.getParameter( "password" );
			m_iLoginTimeout = GetInt( request.getParameter( "timeout" ), 3600 );
			
			if( m_strId.isEmpty( ) )
			{
				m_strErrorMsg = "please insert Id.";
			}
			else if( m_strIp.isEmpty( ) )
			{
				m_strErrorMsg = "please insert IP.";
			}
			else if( m_strUserId.isEmpty( ) )
			{
				m_strErrorMsg = "please insert UserId.";
			}
			else if( m_strPassWord.isEmpty(  ) )
			{
				m_strErrorMsg = "please insert Password.";
			}
			else
			{
				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "INSERT INTO sipserver( Id, Ip, Port, Domain, UserId, PassWord, LoginTimeout ) "
						+ "VALUES( ?, ?, ?, ?, ?, ?, ? )";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strId );
				clsStmt.setString( 2, m_strIp );
				clsStmt.setInt( 3, m_iPort );
				clsStmt.setString( 4, m_strDomain );
				clsStmt.setString( 5, m_strUserId );
				clsStmt.setString( 6, m_strPassWord );
				clsStmt.setInt( 7, m_iLoginTimeout );
				
				clsStmt.executeUpdate( );
				
				m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=SipServerList.jsp\">";
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
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<%= m_strMeta %>
<title>SipServerInsert</title>
</head>
<body>
<b>Add SIP Server</b>
<br><br>
<font color=red><%= m_strErrorMsg %></font>
<form name="sipserver" method="post" action="SipServerInsert.jsp">
<table>
	<tr><td colspan=2>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>Id</b></td>
				<td width="400"><input type="text" name="id" value="<%= m_strId %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>IP</b></td>
				<td width="400"><input type="text" name="ip" value="<%= m_strIp %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>Port</b></td>
				<td width="400"><input type="text" name="port" value="<%= m_iPort %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>Domain</b></td>
				<td width="400"><input type="text" name="domain" value="<%= m_strDomain %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>UserId</b></td>
				<td width="400"><input type="text" name="userid" value="<%= m_strUserId %>" ></td>
			</tr>			
			<tr>
				<td><b>Password</b></td>
				<td><input type="password" name="password" value="<%= m_strPassWord %>" ></td>
			</tr>
			<tr>
				<td width="100"><b>LoginTimeout</b></td>
				<td width="400"><input type="text" name="timeout" value="<%= m_iLoginTimeout %>" ></td>
			</tr>
		</table>
	</td></tr>
	<tr>
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