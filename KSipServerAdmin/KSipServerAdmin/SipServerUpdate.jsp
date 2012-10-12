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
<title>SipServerUpdate</title>
</head>
<body>

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