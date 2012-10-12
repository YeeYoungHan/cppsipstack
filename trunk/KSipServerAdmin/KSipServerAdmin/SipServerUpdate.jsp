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
	String m_strId = request.getParameter( "id" );
	String m_strIp = "";
	int m_iPort = 5060;
	String m_strDomain = "";
	String m_strUserId = "";
	String m_strPassWord = "";
	int m_iLoginTimeout = 3600;
	String m_strErrorMsg = "";
	String m_strMeta = "";
		
	Class.forName("com.mysql.jdbc.Driver");
	
	if( m_strId.isEmpty(  ) )
	{
		m_strErrorMsg = "SipServerId is not found.";
	}
	else if( request.getMethod( ).equals( "POST" ) )
	{
		try
		{
			String strDelete = request.getParameter( "delete" );
			if( strDelete.equals( "Y" ) )
			{
				// SIP 서버 삭제 기능
				m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
				String strSQL = "DELETE FROM sipserver WHERE Id = ?";
				PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
				clsStmt.setString( 1, m_strId );
				
				clsStmt.executeUpdate( );
				
				m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=SipServerList.jsp\">";
			}
			else
			{
				// SIP 서버 수정 기능
				m_strIp = request.getParameter( "ip" );
				m_iPort = GetInt( request.getParameter( "port" ), 5060 );
				m_strDomain = request.getParameter( "domain" );
				m_strUserId = request.getParameter( "userid" );
				m_strPassWord = request.getParameter( "password" );
				m_iLoginTimeout = GetInt( request.getParameter( "timeout" ), 3600 );
				
				if( m_strIp.isEmpty( ) )
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
					String strSQL = "UPDATE sipserver SET Ip = ?, Port = ?, Domain = ?, UserId = ?, PassWord = ?, LoginTimeout = ? WHERE Id = ?";
					PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
					clsStmt.setString( 1, m_strIp );
					clsStmt.setInt( 2, m_iPort );
					clsStmt.setString( 3, m_strDomain );
					clsStmt.setString( 4, m_strUserId );
					clsStmt.setString( 5, m_strPassWord );
					clsStmt.setInt( 6, m_iLoginTimeout );
					clsStmt.setString( 7, m_strId );
					
					clsStmt.executeUpdate( );
					
					m_strMeta = "<meta http-equiv=\"refresh\" content=\"0;url=SipServerList.jsp\">";
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
		// SIP 서버 수정을 위한 사용자 정보 보여주기 기능
		try
		{
			m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
			String strSQL = "SELECT Ip, Port, Domain, UserId, PassWord, LoginTimeout FROM sipserver WHERE Id = ?";
			PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
			clsStmt.setString( 1, m_strId );
			
			ResultSet clsRS = clsStmt.executeQuery( );
			if( clsRS.next(  ) )
			{
				m_strIp = clsRS.getString( 1 );
				m_iPort = clsRS.getInt( 2 );
				m_strDomain = clsRS.getString( 3 );
				m_strUserId = clsRS.getString( 4 );
				m_strPassWord = clsRS.getString( 5 );
				m_iLoginTimeout = clsRS.getInt( 6 );
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
<title>SipServerUpdate</title>
<script>
	function checkDelete()
	{
		if( confirm( "Do you want to Delete <%= m_strId %> ?" ) )
		{
			document.sipserver.delete.value = "Y";
			document.sipserver.submit();
		}
	}
</script>
</head>
<body>
<b>Update SIP Server</b>
<br><br>
<font color=red><%= m_strErrorMsg %></font>
<form name="sipserver" method="post" action="SipServerUpdate.jsp">
<input type="hidden" name="delete" value="N">
<table>
	<tr><td colspan=2>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>Id</b></td>
				<td width="400"><input type="text" name="id" readonly="readonly" value="<%= m_strId %>" ></td>
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