<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%!
	public String GetDateString( String strDate )
	{
		if( strDate == null ) return "";
		if( strDate.length() == 14 )
		{
			String strYear = strDate.substring( 0, 4 );
	    String strMonth = strDate.substring( 4, 6 );
	    String strDay = strDate.substring( 6, 8 );
	    
	    String strHour = strDate.substring( 8, 10 );
	    String strMin = strDate.substring( 10, 12 );
	    String strSec = strDate.substring( 12, 14 );
	    
	    return String.format( "%s-%s-%s %s:%s:%s", strYear, strMonth, strDay, strHour, strMin, strSec );
		}

		return "";
	}
%>
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
<title>CDR List</title>
</head>
<body>
<b>CDR List</b><br>
<br>
<table border=1 cellspacing=0 cellpadding=5>
	<tr bgcolor="#ffcccc" >
		<td width="60" align="center"><b>From</b></td>
		<td width="60" align="center"><b>To</b></td>
		<td width="100" align="center"><b>CallId</b></td>
		<td width="150" align="center"><b>InviteTime</b></td>
		<td width="150" align="center"><b>StartTime</b></td>
		<td width="150" align="center"><b>EndTime</b></td>
		<td width="60" align="center"><b>SipStatus</b></td>
	</tr>
<%
	if( m_clsDbConn != null )
	{
		Statement clsStmt = m_clsDbConn.createStatement( );
		String strSQL = "SELECT FromId, ToId, CallId, InviteTime, StartTime, EndTime, SipStatus FROM sipcdr ORDER BY InviteTime";
		
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
			
			String strInviteTime = GetDateString( clsRS.getString( 4 ) );
			String strStartTime = GetDateString( clsRS.getString( 5 ) );
			String strEndTime = GetDateString( clsRS.getString( 6 ) );
%>
	<tr <%= strBgColor %>>
		<td><%= clsRS.getString( 1 ) %></td>
		<td><%= clsRS.getString( 2 ) %></td>
		<td><%= clsRS.getString( 3 ) %></td>
		<td><%= strInviteTime %></td>
		<td><%= strStartTime %></td>
		<td><%= strEndTime %></td>
		<td align="center"><%= clsRS.getInt( 7 ) %></td>
	</tr>
<%			
		}
	}
%>
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