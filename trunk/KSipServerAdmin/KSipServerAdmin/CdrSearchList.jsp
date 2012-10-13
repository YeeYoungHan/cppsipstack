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
	String m_strFrom = request.getParameter( "from" );
	String m_strTo = request.getParameter( "to" );
	String m_strCallId = request.getParameter( "callid" );
	String m_strInviteTimeStart = request.getParameter( "invitetimestart" );
	String m_strInviteTimeEnd = request.getParameter( "invitetimeend" );
	
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

<table>
	<tr><td align="right">
		<a href="CdrSearch.jsp">Search CDR</a>
	</td></tr>
	<tr><td>

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
		String strSQL = "SELECT FromId, ToId, CallId, InviteTime, StartTime, EndTime, SipStatus "
				+ "FROM sipcdr ";
		int iWhereCount = 0;
		
		if( m_strFrom != null && m_strFrom.isEmpty( ) == false )
		{
			if( iWhereCount == 0 ) strSQL += "WHERE ";
			else strSQL += "AND ";
			
			strSQL += "FromId = ? ";
			++iWhereCount;
		}
		
		if( m_strTo != null && m_strTo.isEmpty( ) == false )
		{
			if( iWhereCount == 0 ) strSQL += "WHERE ";
			else strSQL += "AND ";
			
			strSQL += "ToId = ? ";
			++iWhereCount;
		}

		if( m_strCallId != null && m_strCallId.isEmpty( ) == false )
		{
			if( iWhereCount == 0 ) strSQL += "WHERE ";
			else strSQL += "AND ";
			
			strSQL += "CallId = ? ";
			++iWhereCount;
		}
		
		if( m_strInviteTimeStart != null && m_strInviteTimeStart.isEmpty( ) == false )
		{
			if( iWhereCount == 0 ) strSQL += "WHERE ";
			else strSQL += "AND ";
			
			strSQL += "InviteTime >= ? ";
			++iWhereCount;			
		}
		
		if( m_strInviteTimeEnd != null && m_strInviteTimeEnd.isEmpty( ) == false )
		{
			if( iWhereCount == 0 ) strSQL += "WHERE ";
			else strSQL += "AND ";
			
			strSQL += "InviteTime <= ? ";
			++iWhereCount;			
		}		
		
		strSQL += "ORDER BY InviteTime DESC";
		
		PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
		int iIndex = 0;
		
		if( m_strFrom != null && m_strFrom.isEmpty( ) == false )
		{
			clsStmt.setString( ++iIndex, m_strFrom );
		}

		if( m_strTo != null && m_strTo.isEmpty( ) == false )
		{
			clsStmt.setString( ++iIndex, m_strTo );
		}

		if( m_strCallId != null && m_strCallId.isEmpty( ) == false )
		{
			clsStmt.setString( ++iIndex, m_strCallId );
		}
		
		if( m_strInviteTimeStart != null && m_strInviteTimeStart.isEmpty( ) == false )
		{
			clsStmt.setString( ++iIndex, m_strInviteTimeStart );
		}

		if( m_strInviteTimeEnd != null && m_strInviteTimeEnd.isEmpty( ) == false )
		{
			clsStmt.setString( ++iIndex, m_strInviteTimeEnd );
		}
		
		ResultSet clsRS = clsStmt.executeQuery( );
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
		
		clsRS.close( );
		clsStmt.close( );
	}
%>
</table>
</td></tr>

<tr><td align="center">
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